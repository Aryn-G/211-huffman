# COMP 211 Checkoff 3 - Huffman Coding

Aryan Gera

## Background

This project is for UNC's COMP 211 systems programming course, and its objective is for us to self-learn and present on a topic that interests us. We are to use AI to accelerate our learning. 

I chose the data compression topic, and specifically the huffman coding subtopic, for two main reasons. The first is because of a YouTube trilogy I recently watched from mathematical educator Grant Sanderson, also known as 3Blue1Brown. The trilogy is titled, Compression is Intelligence, and the third and final part actually just released this week (11/17/26). The second is because I remeber learning about huffman back in highschool

## What I am learning

### The motivation

The following scenario is [how 3Blue1Brown introduces huffman](https://youtu.be/l6DKRf-fAAM?si=wT67peOQkL9aiK2F&t=206):

Imagine a robot on a far-away planet which we send instructions to in four directions, `UP`, `DOWN`, `LEFT`, and `RIGHT`. The key thing to note is that all events aren't of equal probability. We send `UP` 1/2 of the time, `DOWN` 1/4 of the time, `LEFT` 1/8 of the time, and `RIGHT` 1/8 of the time: 

| Character | Probability |
| :-: | :-: |
| `UP` | 1/2 |
| `DOWN` | 1/4 |
| `LEFT` | 1/8 |
| `RIGHT` | 1/8 |

#### Naive Approach



#### Huffman Coding

The basic idea behind huffman encoding, in the context of strings, is that characters that appear more frequently should require less bits to be represented. Or, in general contexts, events that appear with higher probabilities should require less information to be represented. 

