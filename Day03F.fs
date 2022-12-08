open System.IO

let ToValue x = 
    let v = (int)x
    if v >= (int)'a' then v-(int)'a' + 1
    else v-(int)'A'+ 27

let SortStr (s:string) = s.ToCharArray() |> Set.ofArray

let SplitIntersect (s:string) = 
    let l = s.Length
    let a, b = SortStr <| s.Substring(0, l/2), SortStr <| s.Substring(l/2)
    let x = Set.intersect a b |> Set.toArray
    x.[0]

let TriMerge (groups, work) n =
    let nWork = Array.append work [|n|]
    match nWork.Length = 3 with | true -> Array.append groups [|nWork|], [||]
                                | _ -> groups, nWork
    
let TriFind (g:Set<char>[]) = 
    let abc = Set.intersect g.[2] (Set.intersect g.[0] g.[1]) |> Set.toArray
    ToValue abc.[0]

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv.[0])
    Array.map(SplitIntersect) data |> Array.fold(fun t x -> t + ToValue x) 0 |> printfn("Part 1: %A")
    let grouped, _ = Array.map(SortStr) data |> Array.fold(TriMerge) ([||],[||])
    printfn("Part 2: %A") <| Array.fold(fun t x -> t + TriFind x) 0 grouped
    0
