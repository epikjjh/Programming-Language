-- Flipping coins
fliplist :: String -> String
fliplist [] = []
fliplist (x:xs) = fliplist xs ++ [reverseface x]

-- Reverse coin's face
reverseface :: Char -> Char
reverseface face
    | face == 'H' = 'T'
    | face == 'T' = 'H'

-- Find where to flip
findplace :: Int -> String -> Int
findplace n (x:xs)
    | (x == (head xs)) = (findplace n xs)
    | otherwise = n - (length xs)

-- Flip stack segment
flipstack :: Int -> String -> String
flipstack n input = (fliplist (take (findplace n input) input)) ++ (drop (findplace n input) input)

-- Main process
flipcoin :: String -> [Int]
flipcoin list
    | list == (take (length list) (repeat 'H')) = [0]
    | list == (take (length list) (repeat 'T')) = [(length list),0]
    | otherwise = (findplace (length list) list) : flipcoin(flipstack (length list) list)
