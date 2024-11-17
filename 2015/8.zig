const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const eql = std.mem.eql;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    var code: usize = 0;
    var memory: usize = 0;
    var encoded: usize = 0;

    var t = tokenize(u8, in, "\n");
    while (t.next()) |line| {
        code += line.len;

        var i: usize = 1;
        while (i < line.len - 1) {
            switch (line[i]) {
                '\\' => switch (line[i + 1]) {
                    '"', '\\' => i += 2,
                    'x' => i += 4,
                    else => unreachable,
                },
                else => i += 1,
            }
            memory += 1;
        }

        encoded += line.len + 2;
        for (line) |c| {
            switch (c) {
                '"', '\\' => encoded += 1,
                else => {},
            }
        }
    }

    try out.print("{}\n", .{code - memory});
    try out.print("{}\n", .{encoded - code});
}

const expectEqual = std.testing.expectEqual;

test "Part One" {}

test "Part Two" {}
