const std = @import("std");

const t = std.testing;

pub fn main() !void {
    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    try out.print("{}\n", .{whatFloor(in)});
    try out.print("{}\n", .{whatPosition(in)});
}

fn whatFloor(in: []const u8) i32 {
    var floor: i32 = 0;
    for (in) |c|
        switch (c) {
            '(' => floor += 1,
            ')' => floor -= 1,
            else => unreachable,
        };
    return floor;
}

test "Part One" {
    try t.expectEqual(0, whatFloor("(())"));
    try t.expectEqual(0, whatFloor("()()"));
    try t.expectEqual(3, whatFloor("((("));
    try t.expectEqual(3, whatFloor("(()(()("));
    try t.expectEqual(3, whatFloor("))((((("));
}

fn whatPosition(in: []const u8) usize {
    var floor: i32 = 0;
    for (in, 1..) |c, i|
        switch (c) {
            '(' => floor += 1,
            ')' => {
                if (floor == 0) return i;
                floor -= 1;
            },
            else => unreachable,
        };

    unreachable;
}

test "Part Two" {
    try t.expectEqual(1, whatPosition(")"));
    try t.expectEqual(5, whatPosition("()())"));
}
