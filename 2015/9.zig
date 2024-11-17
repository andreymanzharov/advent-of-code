const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const eql = std.mem.eql;
const reverse = std.mem.reverse;
const swap = std.mem.swap;
const tokenize = std.mem.tokenize;

const Edge = struct { to: u32, dist: u32 };
const Graph = std.ArrayList(std.ArrayList(Edge));
const Visited = std.StaticBitSet(64);

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const alloc = gpa.allocator();

    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    var ids = std.StringHashMap(u32).init(alloc);
    defer ids.deinit();

    var g: [8][8]u32 = .{.{0} ** 8} ** 8;

    var t = tokenize(u8, in, " \n");
    while (t.next()) |fromCity| {
        _ = t.next().?;
        const toCity = t.next().?;
        _ = t.next().?;
        const distance = parseInt(u32, t.next().?, 10) catch unreachable;

        const from = blk: {
            const e = try ids.getOrPut(fromCity);
            if (!e.found_existing)
                e.value_ptr.* = ids.count() - 1;
            break :blk e.value_ptr.*;
        };
        const to = blk: {
            const e = try ids.getOrPut(toCity);
            if (!e.found_existing)
                e.value_ptr.* = ids.count() - 1;
            break :blk e.value_ptr.*;
        };
        g[from][to] = distance;
        g[to][from] = distance;
    }

    var route = try std.ArrayList(u32).initCapacity(alloc, ids.count());
    var i: u32 = 0;
    while (i < ids.count()) : (i += 1)
        try route.append(@intCast(i));

    var min = routeDistance(route.items, &g);
    var max = min;

    while (nextPermutation(route.items)) {
        const d = routeDistance(route.items, &g);
        min = @min(min, d);
        max = @max(max, d);
    }

    try out.print("{}\n", .{min});
    try out.print("{}\n", .{max});
}

fn routeDistance(arr: []u32, g: *[8][8]u32) u32 {
    var res: u32 = 0;
    for (arr[1..], 1..) |x, i| {
        res += g[arr[i - 1]][x];
    }
    return res;
}

fn nextPermutation(arr: []u32) bool {
    var i = arr.len - 2;
    const pivot = while (i < arr.len) : (i -= 1) {
        if (arr[i] < arr[i + 1])
            break i;
    } else return false;
    i = arr.len - 1;
    while (i > pivot) : (i -= 1) {
        if (arr[i] > arr[pivot]) {
            swap(u32, &arr[i], &arr[pivot]);
            break;
        }
    }
    reverse(u32, arr[pivot + 1 ..]);
    return true;
}

const expectEqual = std.testing.expectEqual;

test "Part One" {}

test "Part Two" {}
