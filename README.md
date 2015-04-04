#recommendation

----

 It's travel destination project repository maintained by Yanqiao Zhan and Haichen Zhu.
--------------
  communication between front-end and back-end:
 * POPULAR
*  Require for most popular destination: post a "popular"
*  Answer to most popular destination: ordered destination id(from most popular to least popular)json data  
{  
   "id1":1,
   "id2":2,
   "id3":3
}

 *  RECOMMEND
* Require for recommendation based on user selected popular: post a json data the name of json is "select", the key is destination id, value is -1
{  
   "id1":-1,
   "id2":-1,
   "id3":-1
}
* Answer to recommendation using json data, the key is destination id and value is its sort order
{  
   "id1":1,
   "id2":2,
   "id3":3,
   "id4":4,
   "id5":5
}
