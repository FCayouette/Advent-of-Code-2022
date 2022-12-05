open System.IO

[<EntryPoint>]
let main argv =
    let pairs = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string) -> ((int)(s.[0]) - (int)'A', (int)(s.[2])-(int)'X'))
    let part1, part2 = Array.fold(fun (t1, t2) (a,b) -> (t1 + b+1+3*((4+b-a)%3), t2 + 3*b+(a+b+2)%3+1)) (0,0) pairs
    printfn("Part 1: %A") <| part1
    printfn("Part 2: %A") <| part2
    0
