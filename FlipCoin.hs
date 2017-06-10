-- Flipping coins
fliplist :: String -> String
fliplist [] = []
fliplist (x:xs) = reverselist xs ++ [reverseface x]

-- Reverse coin's face
reverseface :: Char -> Char
reverseface face
    | face == 'H' = 'T'
    | face == 'T' = 'H'

flipcoin :: String -> [Int]
flipcoin input
    | (length input) < 2 = error "Too short input"
    | (take (length input) (repeat 'H')) == input = [0]
  --  | otherwise =  
