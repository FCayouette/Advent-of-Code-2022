open System.IO

let ToInt = System.Int32.Parse

let ParseInstruction (s:string) = 
    let tokens = s.Split(' ')
    ToInt tokens[1], ToInt tokens[3] - 1, ToInt tokens[5] - 1

let Reduce (s:string) =
    s.ToCharArray() |> Array.indexed |> Array.filter(fun (i, _) -> i % 4 = 1) |> Array.map snd

let ParseStacks s =
    s |> Array.map Reduce |> Array.transpose |> Array.map(fun a -> Array.filter(fun c -> c <> ' ') a)

let SplitData s =
    let index = Array.findIndex(fun (x:string) -> x.Length <= 1) s
    Array.rev s[..index-2] |> ParseStacks, Array.map ParseInstruction s[(index+1)..]

let Part1 count from dest =
    let mutable workF = from
    let mutable workD = dest
    for _ in 1 .. count do
        workD <- Array.append workD [|Array.last workF|]
        workF <- Array.removeAt (workF.Length-1) workF
    workF, workD

let Part2 c (from: char[]) dest =
    Array.copy from[0 .. (from.Length - c - 1)], Array.append dest from[from.Length - c ..]

let GetResult stacks =
    System.String(Array.fold(fun result (t:char[]) -> Array.append result [|t[t.Length-1]|]) [||] stacks)

let DoPart init insts operation = 
    let stacks = Array.copy init
    Array.iter(fun (c,f,d) -> let x, y = operation c stacks[f] stacks[d]
                              stacks[f]<-x
                              stacks[d]<-y) insts
    GetResult stacks

[<EntryPoint>]
let main argv =
    let init, instructions = File.ReadAllLines(argv[0]) |> SplitData
    printfn "Part 1: %A" <| DoPart init instructions Part1
    printfn "Part 2: %A" <| DoPart init instructions Part2
    0
   
