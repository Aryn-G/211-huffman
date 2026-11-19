# COMP 211 Checkoff 3 - Huffman Coding

Aryan Gera

## Background

This project is for UNC's COMP 211 systems programming course, and its objective is for us to self-learn and present on a topic that interests us. We were to use AI to accelerate our learning and understanding of the topic. 

I chose the data compression topic, and specifically the huffman coding subtopic, for two main reasons. The first is because of a YouTube trilogy I recently watched from mathematical educator Grant Sanderson, also known as [3Blue1Brown](https://www.youtube.com/@3blue1brown). The trilogy is titled, _Compression is Intelligence_, and the third and final part actually just released this week (11/17/26). The second is because I remeber learning about huffman back in highschool, and I always wanted to learn more about it. 

## The Motivation

The following scenario is [how 3Blue1Brown introduces compression in Part 1](https://youtu.be/l6DKRf-fAAM?si=wT67peOQkL9aiK2F&t=206) of his series mentioned above:

Imagine a robot on a far-away planet which we send instructions to in four directions, `UP`, `DOWN`, `LEFT`, and `RIGHT`. The key thing to note is that all events aren't of equal probability. We send `UP` 1/2 of the time, `DOWN` 1/4 of the time, `LEFT` 1/8 of the time, and `RIGHT` 1/8 of the time: 

| Event $E$ | Probability $P(E)$ |
| :-: | :-: |
| `UP` | 1/2 |
| `DOWN` | 1/4 |
| `LEFT` | 1/8 |
| `RIGHT` | 1/8 |

The naive approach would be to simply use 2-bits for each one of 4 events. This way, when the robot recieves the encoded string, it can simply read chunks of 2-bits and know what moves to perform. 

This approach uses:

$$
(2 \text{ bits} \times P(\text{UP})) + (2 \text{ bits} \times P(\text{DOWN})) + \\
(2 \text{ bits} \times P(\text{LEFT})) + (2 \text{ bits} \times P(\text{RIGHT})) = \\
2 \text{ bits per event on average}
$$

However, this approach is not optimal. The inefficiency with this approach is that it doesn't take the probabilties of each event into account. That is where huffman, and many other compression algorithms come in.

## Huffman Coding

### The Idea

The basic idea behind huffman coding, in the context of strings, is that characters that appear more frequently should require less bits to be represented. Or, in general contexts, events that appear with higher probabilities should require less information to be represented. 

In the senario above, we see that `UP` has the highest probability of occuring, so we would want to represent it with the least number of bits. `DOWN` has the second highest, so we represent it with the second least number of bits and so on. 

Suppose we use the following bit representation for each event:

| Event $E$ | Probability $P(E)$ | Bit Representation |
| :-: | :-: | :-: |
| `UP` | 1/2 | 0 |
| `DOWN` | 1/4 | 10 |
| `LEFT` | 1/8 | 110 |
| `RIGHT` | 1/8 | 111 |

This approach uses:

$$
(1 \text{ bit} \times P(\text{UP})) + (2 \text{ bits} \times P(\text{DOWN})) + \\
(3 \text{ bits} \times P(\text{LEFT})) + (3 \text{ bits} \times P(\text{RIGHT})) = \\
1.75 \text{ bits per event on average}
$$

When our robot recieves the encoded bit string `01011010111`, we run into a problem the naive approach didn't have to worry about: how does the robot know where to split up the string into the original events? 

The answer is in how we chose those bits. We specifically chose those bits so that they are what is known as **prefix-free**. Notice that when we see a `0`, we know that it must be `UP`, because no other event starts with a `0`. When we see a `1`, we know that it must be either `DOWN`, `LEFT`, or `RIGHT`, but we don't know which one yet. So we check the next bit, if it is a `0`, then we know it must be `DOWN`, because no other event starts with `10`. If the next bit is a `1`, then we know it must be either `LEFT` or `RIGHT`, and the next bit will tell us which one.

We can represent this with a binary tree, where each fork represents a set of possible events. In this tree, each left edge represents a `0` and each right edge represents a `1`. 

```
        *
      0/ \1
      UP  *
        0/ \1
      DOWN  *
          0/ \1
       LEFT   RIGHT
```

If we simply start at the root and traverse the tree down to an event, we can see the path is our bit representation for that event.

### The Algorithm


## Coding

### Design Decisions

Coding up a project like this involves having a clear understanding of what I wanted to learn and desmonstrate. I started by brain dumping what I wanted and how I wanted things to be:

![google docs brain dump screenshot](./images/image.png)

To summarize, I wanted to have one program, which depending on the arguments passed in, could either encode or decode a given input. Originally, I wanted file support as well, but I realized that would complicate the scope of the project beyond what I wanted to learn and demonstrate. 

While chatting with AI and learning about how Huffman is implemented, I ran into a few design decisions that I had to make.

#### Element Priorities

Firstly, when popping the two elements of minimum priority, I had to decide between a minimum priority queue (min-heap) or a two-queue method:

- Using the Min-Heap 

- Using the Two-Queue Method

Both methods are $O(n \log n)$, however, I originally thought, they both would be just as  complicated to implement as the other, so I should just stick with the data structure we all learn in earlier classes.

However, after discussing with AI, I realized that the two-queue method was much simpler to implement. The two-queue method's first queue stores a sorted list of the original events (this is where the $O(n \log n)$ comes from), and the second queue is used for stored merged nodes. The key insight is that the first queue never pushes after the initial insertions, and that the second queue never needs to grow beyond the number of events. This means that we can use a simple array to represent both queues, and simply keep track of the front and back indices of each queue.

So, appending is `q[tail++] = x`, and popping is `x = q[head++]`. 

#### Tree Representation
The second issue I ran into was how to even represent a tree in C. After some research, I found that there are two main methods of doing so:

- Struct pointers method 
    - more intuitive
    - but memory is all over the place, having to deal with mallocs and frees

- In-place nodes array 
    - less intuitive at first
    - but memory is contiguous, and easier to manage

My original thought was to use the more intuitive, struct pointers method, but after discussing with AI, I realized that the in-place node array method was more efficient and easier to manage for the following reasons. Firstly, for huffman, the number of nodes in the tree is always $2n - 1$, where $n$ is the number of events or characters. This means we know beforehand exactly how many nodes we will need, so there is no reason to mess with mallocs and frees. Secondly, since we are building the tree from the bottom up, we don't have to worry about moving child pointers around, which was the main benefit of using the struct method in the first place. 

