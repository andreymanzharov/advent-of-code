let input = Utils.read_lines "input"

let score1 = function
  | "A X" -> 3 + 1
  | "A Y" -> 6 + 2
  | "A Z" -> 0 + 3
  | "B X" -> 0 + 1
  | "B Y" -> 3 + 2
  | "B Z" -> 6 + 3
  | "C X" -> 6 + 1
  | "C Y" -> 0 + 2
  | "C Z" -> 3 + 3
  | _ -> 0

let part1 = List.fold_left (fun acc round -> acc + score1 round) 0 input

let score2 = function
  | "A X" -> 0 + 3
  | "A Y" -> 3 + 1
  | "A Z" -> 6 + 2
  | "B X" -> 0 + 1
  | "B Y" -> 3 + 2
  | "B Z" -> 6 + 3
  | "C X" -> 0 + 2
  | "C Y" -> 3 + 3
  | "C Z" -> 6 + 1
  | _ -> 0

let part2 = List.fold_left (fun acc round -> acc + score2 round) 0 input

let () =
  print_int part1;
  print_newline ();
  print_int part2;
  print_newline ()
