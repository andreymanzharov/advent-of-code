const std = @import("std");

pub fn build(b: *std.Build) !void {
    const session = try std.process.getEnvVarOwned(b.allocator, "AOC_SESSION");
    defer b.allocator.free(session);

    var cookieBuf: [8 + 128]u8 = undefined;
    const cookie = try std.fmt.bufPrint(&cookieBuf, "session={s}", .{session});
    defer b.allocator.free(cookie);

    const Day = struct {
        mtime: i128,
        run: *std.Build.Step.Run,
        tests: *std.Build.Step.Run,
    };

    var days: [26]?Day = .{null} ** 26;

    {
        const year = std.fs.path.basename(b.build_root.path.?);

        var day: u8 = 1;
        while (day <= 25) : (day += 1) {
            var source_file_buf: [16]u8 = undefined;

            const source_file = try std.fmt.bufPrint(&source_file_buf, "{}.zig", .{day});
            const stat = b.build_root.handle.statFile(source_file) catch |err| switch (err) {
                error.FileNotFound => continue,
                else => return err,
            };

            days[day] = .{
                .mtime = stat.mtime,
                .run = runDay(b, year, source_file, cookie),
                .tests = testDay(b, source_file),
            };
        }
    }

    const run_step = b.step("run", "Run solutions");

    const all = b.option(bool, "all", "All days at once") orelse false;

    if (all) {
        for (days) |day_opt| if (day_opt) |day| {
            run_step.dependOn(&day.run.step);
        };
    } else {
        const test_step = b.step("test", "Run unit tests");

        var last_modified_day: usize = 0;
        for (days[1..], 1..) |day_opt, idx| {
            if (day_opt) |day| {
                if (last_modified_day == 0 or days[last_modified_day].?.mtime < day.mtime) {
                    last_modified_day = idx;
                }
            }
        }
        if (last_modified_day > 0) {
            test_step.dependOn(&days[last_modified_day].?.tests.step);
            run_step.dependOn(&days[last_modified_day].?.run.step);
        }
    }

    b.default_step = run_step;
}

const BuildDaySteps = struct {
    run_exe: *std.Build.Step.Run,
    run_tests: *std.Build.Step.Run,
};

fn runDay(b: *std.Build, year: []const u8, source_file: []const u8, cookie: []const u8) *std.Build.Step.Run {
    const day = std.fs.path.stem(source_file);

    const curl = b.addSystemCommand(&.{ "curl", "--cookie", cookie, "--output" });
    const input = curl.addOutputFileArg("input.txt");
    var url_buf: [64]u8 = undefined;
    const url = std.fmt.bufPrint(&url_buf, "https://adventofcode.com/{s}/day/{s}/input", .{ year, day }) catch unreachable;
    curl.addArgs(&.{url});

    const exe = b.addExecutable(.{
        .name = day,
        .root_source_file = b.path(source_file),
        .target = b.host,
        .optimize = .ReleaseFast,
    });
    exe.root_module.addAnonymousImport("input", .{
        .root_source_file = input,
    });

    return b.addRunArtifact(exe);
}

fn testDay(b: *std.Build, source_file: []const u8) *std.Build.Step.Run {
    const tests = b.addTest(.{
        .root_source_file = b.path(source_file),
        .target = b.host,
    });
    return b.addRunArtifact(tests);
}
