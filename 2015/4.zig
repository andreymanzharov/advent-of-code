const std = @import("std");

const dbg = std.debug.print;
const parseInt = std.fmt.parseInt;
const tokenize = std.mem.tokenize;

pub fn main() !void {
    const in = std.mem.trimRight(u8, @embedFile("input"), &std.ascii.whitespace);
    const out = std.io.getStdOut().writer();

    const Md5 = std.crypto.hash.Md5;

    var hash: [Md5.digest_length]u8 = undefined;

    var buf: [10]u8 = undefined;
    var key: u32 = 1;
    var secret_key1: u32 = 0;
    var secret_key2: u32 = 0;
    while (secret_key1 == 0 or secret_key2 == 0) : (key += 1) {
        var md5 = Md5.init(.{});
        md5.update(in);
        md5.update(try std.fmt.bufPrint(&buf, "{}", .{key}));
        md5.final(&hash);
        if (hash[0] == 0 and hash[1] == 0) {
            if (hash[2] < 16 and secret_key1 == 0)
                secret_key1 = key;
            if (hash[2] == 0 and secret_key2 == 0)
                secret_key2 = key;
        }
    }
    try out.print("{}\n{}\n", .{ secret_key1, secret_key2 });
}

const expectEqual = std.testing.expectEqual;

test "Part One" {}

test "Part Two" {}
