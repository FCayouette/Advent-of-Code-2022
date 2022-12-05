open System.IO

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv.[0])
    let almost, last = Array.fold(fun (elves, t) (s:string)-> match s.Length with 
                            | 0 -> (Array.append elves [|t|], 0)
                            | _ -> (elves, t + (System.Int32.Parse s))) ([||], 0) data
    let elves = Array.sortDescending (Array.append almost [|last|])

    printfn "Part 1: %A" <| elves.[0]
    printfn "Part 2: %A" <| Array.sum elves.[0..2]
    0 
