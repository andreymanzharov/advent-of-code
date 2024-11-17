const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const alloc = gpa.allocator();

    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    try out.print("{}\n", .{try housesAtLeastOnePresent1(in, alloc)});
    try out.print("{}\n", .{try housesAtLeastOnePresent2(in, alloc)});
}

fn housesAtLeastOnePresent1(in: []const u8, alloc: std.mem.Allocator) !usize {
    var houses = std.AutoHashMap(struct { i16, i16 }, void).init(alloc);
    defer houses.deinit();

    var x: i16 = 0;
    var y: i16 = 0;
    try houses.put(.{ x, y }, {});
    for (in) |d| {
        switch (d) {
            '^' => y += 1,
            'v' => y -= 1,
            '>' => x += 1,
            '<' => x -= 1,
            else => unreachable,
        }
        try houses.put(.{ x, y }, {});
    }

    return houses.count();
}

fn housesAtLeastOnePresent2(in: []const u8, alloc: std.mem.Allocator) !usize {
    var houses = std.AutoHashMap(struct { i16, i16 }, void).init(alloc);
    defer houses.deinit();

    try houses.put(.{ 0, 0 }, {});

    var x = [_]i16{ 0, 0 };
    var y = [_]i16{ 0, 0 };
    for (in, 0..) |d, i| {
        switch (d) {
            '^' => y[i & 1] += 1,
            'v' => y[i & 1] -= 1,
            '>' => x[i & 1] += 1,
            '<' => x[i & 1] -= 1,
            else => unreachable,
        }
        try houses.put(.{ x[i & 1], y[i & 1] }, {});
    }

    return houses.count();
}

const expectEqual = std.testing.expectEqual;

test "Part One" {
    try expectEqual(2, try housesAtLeastOnePresent1("^v", std.testing.allocator));
    try expectEqual(4, try housesAtLeastOnePresent1("^>v<", std.testing.allocator));
    try expectEqual(2, try housesAtLeastOnePresent1("^v^v^v^v^v", std.testing.allocator));
}

test "Part Two" {
    try expectEqual(3, try housesAtLeastOnePresent2("^v", std.testing.allocator));
    try expectEqual(3, try housesAtLeastOnePresent2("^>v<", std.testing.allocator));
    try expectEqual(11, try housesAtLeastOnePresent2("^v^v^v^v^v", std.testing.allocator));
}
