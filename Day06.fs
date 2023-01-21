open System.IO

let FindUniqueSequence (s:'a[]) len =
    let indices = {len - 1 .. s.Length - 1}
    Seq.fold(fun r i -> match r <> -1 with 
                  true -> r 
                | false -> let x = Array.distinct s[i-(len-1)..i]
                           match x.Length = len with true -> i+1 | _ -> -1) -1 indices
        
[<EntryPoint>]
let main argv =
    let data = [|for c in File.ReadAllLines(argv[0])[0] -> c|]
    printfn "Part 1: %A" <| FindUniqueSequence data 4
    printfn "Part 2: %A" <| FindUniqueSequence data 14
    0