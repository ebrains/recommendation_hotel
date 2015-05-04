#recommendation

----

 It's hotel recommender system maintained by Yanqiao Zhan and Haichen Zhu.
--------------

For the users who rate less than 4 times, we ignore them.
For the anonymous uers, like "A TripAdvisor Member", we ignore them.

We use matrix normalizing to deal with sparse problem. For every non-empty cell, substract it with the column average value and row average value; for empty cell, keep it as 0.
 
We use item-based CF, use adjusted cosine to calculate the similarities between every two items.

If result is less than k(we want to return top k), simply merge most popular and item-based CF, return merged answer.


 
