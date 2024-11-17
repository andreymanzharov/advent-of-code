const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const eql = std.mem.eql;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    var grid1: [1000][1000]bool = .{.{false} ** 1000} ** 1000;
    var grid2: [1000][1000]u8 = .{.{0} ** 1000} ** 1000;

    const Inst = enum { turn_on, toggle, turn_off };

    var t = tokenize(u8, in, " ,\n");
    while (t.peek() != null) {
        const inst: Inst = if (eql(u8, "turn", t.next().?))
            if (eql(u8, "on", t.next().?)) .turn_on else .turn_off
        else
            .toggle;
        const x1 = try parseInt(u16, t.next().?, 10);
        const y1 = try parseInt(u16, t.next().?, 10);
        _ = t.next();
        const x2 = try parseInt(u16, t.next().?, 10);
        const y2 = try parseInt(u16, t.next().?, 10);
        var x = x1;
        while (x <= x2) : (x += 1) {
            var y = y1;
            while (y <= y2) : (y += 1) switch (inst) {
                .turn_on => {
                    grid1[x][y] = true;
                    grid2[x][y] += 1;
                },
                .toggle => {
                    grid1[x][y] = !grid1[x][y];
                    grid2[x][y] += 2;
                },
                .turn_off => {
                    grid1[x][y] = false;
                    if (grid2[x][y] > 0)
                        grid2[x][y] -= 1;
                },
            };
        }
    }

    var lights: i32 = 0;
    for (grid1) |row| {
        for (row) |lit| {
            if (lit) {
                lights += 1;
            }
        }
    }

    var brightness: i32 = 0;
    for (grid2) |row| {
        for (row) |light| {
            brightness += light;
        }
    }

    try out.print("{}\n", .{lights});
    try out.print("{}\n", .{brightness});
}

const expectEqual = std.testing.expectEqual;

test "Part One" {}

test "Part Two" {}
