open System.IO

let ParseData (s:string) =
    let pairs = s.Split(',') |> Array.map(fun (x:string) -> let a = x.Split('-') |> Array.map System.Int32.Parse
                                                            a.[0], a.[1])
    pairs.[0], pairs.[1]

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv.[0]) |> Array.map ParseData
    Array.filter(fun (f, s) -> (fst f <= fst s && snd f >= snd s) || fst s <= fst f && snd s >= snd f) data |> Array.length |> printfn "Part 1: %A"
    Array.filter(fun (first, second) -> not (fst second > snd first || fst first > snd second)) data |> Array.length |> printfn "Part 2: %A"
    0
