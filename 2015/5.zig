const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    var count1: i32 = 0;
    var count2: i32 = 0;

    var t = tokenize(u8, in, "\n");
    while (t.next()) |line| {
        var vowels: i32 = if (std.mem.indexOfScalar(u8, "aeiou", line[0]) == null) 0 else 1;
        var twice = false;
        var no_ab_cd_pq_xy = true;
        for (line[1..], 1..) |c, i| {
            switch (c) {
                'a', 'e', 'i', 'o', 'u' => vowels += 1,
                'b' => no_ab_cd_pq_xy = no_ab_cd_pq_xy and line[i - 1] != 'a',
                'd' => no_ab_cd_pq_xy = no_ab_cd_pq_xy and line[i - 1] != 'c',
                'q' => no_ab_cd_pq_xy = no_ab_cd_pq_xy and line[i - 1] != 'p',
                'y' => no_ab_cd_pq_xy = no_ab_cd_pq_xy and line[i - 1] != 'x',
                else => {},
            }
            twice = twice or c == line[i - 1];
        }
        if (vowels >= 3 and twice and no_ab_cd_pq_xy)
            count1 += 1;

        var two_twice = false;
        var two_between = false;
        for (line[2..], 2..) |c, i| {
            if (!two_twice and std.mem.indexOf(u8, line[0..i], line[i .. i + 2]) != null)
                two_twice = true;
            if (line[i - 2] == c)
                two_between = true;
            if (two_twice and two_between) {
                count2 += 1;
                break;
            }
        }
    }

    try out.print("{}\n", .{count1});
    try out.print("{}\n", .{count2});
}

const expectEqual = std.testing.expectEqual;

test "Part One" {}

test "Part Two" {}
