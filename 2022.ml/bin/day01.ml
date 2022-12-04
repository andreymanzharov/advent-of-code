let input = Utils.read_lines "input"

let next_elf_calories data =
  let rec helper data acc =
    match data with
    | [] -> (acc, [])
    | "" :: rest -> (acc, rest)
    | calory :: rest -> helper rest (acc + int_of_string calory)
  in
  match data with [] -> None | _ -> Some (helper data 0)

let elf_calories = Seq.unfold next_elf_calories
let part1 = Seq.fold_left max 0 (elf_calories input)

let part2 =
  let max3 m x =
    match m with
    | m1, m2, _ when x > m1 -> (x, m1, m2)
    | m1, m2, _ when x > m2 -> (m1, x, m2)
    | m1, m2, m3 when x > m3 -> (m1, m2, x)
    | _ -> m
  in
  let m1, m2, m3 = Seq.fold_left max3 (0, 0, 0) (elf_calories input) in
  m1 + m2 + m3

let () =
  print_int part1;
  print_newline ();
  print_int part2;
  print_newline ()
