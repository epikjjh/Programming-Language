isprime :: Int -> Bool
isprime n = (length compare_list == 1)
    where
    -- Use definition of prime number
    compare_list = [x|x <- [1..n-1], n `mod` x == 0] 

-- Check if given number is positive
ispositive :: Int -> Bool
ispositive x = (x > 0)

findingprimes :: Int -> Int -> [Int]
findingprimes n m
-- n could be negative number. However m must not be negative number
    | m < 0 = error "Input argument2 must not be negative number"
    | otherwise = take m (filter isprime (filter ispositive [n..]))
