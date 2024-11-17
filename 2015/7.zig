const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const eql = std.mem.eql;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const alloc = gpa.allocator();

    const in = @embedFile("input");
    const out = std.io.getStdOut().writer();

    var inst = try parse(alloc, in);
    defer inst.deinit();

    const a = find(&inst, "a", alloc);
    try out.print("{}\n", .{a});

    try inst.put("b", .{ .value = a });
    const new_a = find(&inst, "a", alloc);
    try out.print("{}\n", .{new_a});
}

fn parseArg(arg: []const u8, alloc: std.mem.Allocator) *Signal {
    const signal = alloc.create(Signal) catch unreachable;
    if (std.ascii.isDigit(arg[0]))
        signal.* = .{ .value = parseInt(u16, arg, 10) catch unreachable }
    else
        signal.* = .{ .wire = arg };
    return signal;
}

fn parse(alloc: std.mem.Allocator, in: []const u8) !std.StringHashMap(Signal) {
    var inst = std.StringHashMap(Signal).init(alloc);
    errdefer inst.deinit();

    var t = tokenize(u8, in, " \n");
    while (t.next()) |token| {
        const signal: Signal = blk: {
            if (eql(u8, "NOT", token))
                break :blk .{ .gate = .{ .not = parseArg(t.next().?, alloc) } };
            const arg = parseArg(token, alloc);
            if (eql(u8, "->", t.peek().?)) {
                defer alloc.destroy(arg);
                break :blk arg.*;
            }
            const ty = t.next().?;
            if (eql(u8, "AND", ty))
                break :blk .{ .gate = .{ .@"and" = .{ .left = arg, .right = parseArg(t.next().?, alloc) } } };
            if (eql(u8, "OR", ty))
                break :blk .{ .gate = .{ .@"or" = .{ .left = arg, .right = parseArg(t.next().?, alloc) } } };
            if (eql(u8, "LSHIFT", ty))
                break :blk .{ .gate = .{ .lshift = .{ .value = arg, .shift = parseInt(u4, t.next().?, 10) catch unreachable } } };
            if (eql(u8, "RSHIFT", ty))
                break :blk .{ .gate = .{ .rshift = .{ .value = arg, .shift = parseInt(u4, t.next().?, 10) catch unreachable } } };
            unreachable;
        };
        _ = t.next();
        const wire = t.next().?;
        try inst.put(wire, signal);
    }

    return inst;
}

const Signal = union(enum) {
    gate: Gate,
    wire: []const u8,
    value: u16,
};

const Gate = union(enum) {
    not: *const Signal,
    @"and": struct { left: *const Signal, right: *const Signal },
    @"or": struct { left: *const Signal, right: *const Signal },
    lshift: struct { value: *const Signal, shift: u4 },
    rshift: struct { value: *const Signal, shift: u4 },
};

fn emulate(inst: *const std.StringHashMap(Signal), signal: *const Signal, cache: *std.StringHashMap(u16)) u16 {
    const value = switch (signal.*) {
        .gate => |gate| switch (gate) {
            .not => |arg| ~emulate(inst, arg, cache),
            .@"and" => |args| emulate(inst, args.left, cache) & emulate(inst, args.right, cache),
            .@"or" => |args| emulate(inst, args.left, cache) | emulate(inst, args.right, cache),
            .lshift => |args| emulate(inst, args.value, cache) << args.shift,
            .rshift => |args| emulate(inst, args.value, cache) >> args.shift,
        },
        .wire => |from| blk: {
            if (cache.get(from)) |cached|
                break :blk cached;
            const value = emulate(inst, &inst.get(from).?, cache);
            cache.put(from, value) catch unreachable;
            break :blk value;
        },
        .value => |value| value,
    };
    return value;
}

fn find(inst: *const std.StringHashMap(Signal), wire: []const u8, alloc: std.mem.Allocator) u16 {
    var signal: Signal = .{ .wire = wire };
    var cache = std.StringHashMap(u16).init(alloc);
    defer cache.deinit();
    return emulate(inst, &signal, &cache);
}

const expectEqual = std.testing.expectEqual;

test "Part One" {
    var arena = std.heap.ArenaAllocator.init(std.testing.allocator);
    defer arena.deinit();

    const alloc = arena.allocator();

    var inst = try parse(alloc,
        \\123 -> x
        \\456 -> y
        \\x AND y -> d
        \\x OR y -> e
        \\x LSHIFT 2 -> f
        \\y RSHIFT 2 -> g
        \\NOT x -> h
        \\NOT y -> i
    );
    defer inst.deinit();

    try expectEqual(72, find(&inst, "d", alloc));
    try expectEqual(507, find(&inst, "e", alloc));
    try expectEqual(492, find(&inst, "f", alloc));
    try expectEqual(114, find(&inst, "g", alloc));
    try expectEqual(65412, find(&inst, "h", alloc));
    try expectEqual(65079, find(&inst, "i", alloc));
    try expectEqual(123, find(&inst, "x", alloc));
    try expectEqual(456, find(&inst, "y", alloc));
}

test "Part Two" {}
