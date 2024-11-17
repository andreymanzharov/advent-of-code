const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    var t = tokenize(u8, in, "x\n");

    var square: u32 = 0;
    var ribbon: u32 = 0;
    while (t.peek() != null) {
        const l = try parseInt(u32, t.next().?, 10);
        const w = try parseInt(u32, t.next().?, 10);
        const h = try parseInt(u32, t.next().?, 10);

        square += 2 * (l * w + l * h + w * h) + @min(l * w, l * h, w * h);
        ribbon += 2 * @min(l + w, l + h, w + h) + l * w * h;
    }

    try out.print("{}\n", .{square});
    try out.print("{}\n", .{ribbon});
}

test "Part One" {}

test "Part Two" {}
