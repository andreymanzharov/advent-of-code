const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const eql = std.mem.eql;
const reverse = std.mem.reverse;
const swap = std.mem.swap;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const alloc = gpa.allocator();
    _ = alloc; // autofix

    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    try out.print("{s}\n", .{in});
}

const expectEqual = std.testing.expectEqual;

test "Part One" {}

test "Part Two" {}
