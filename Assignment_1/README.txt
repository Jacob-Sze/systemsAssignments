Systems Programming Assignment 1 Write up - My little malloc()
__________________________________________________________

Jacob Sze - jms1200@scarletmail.rutgers.edu
Neil Patel - np964@scarletmail.rutgers.edu

__________________________________________________________
Our Test Plan

All test cases outside of those for performance testing are in their own C files and can be run individually. 

Error Testing

For error detection, we utilized three of the given usage errors in the writeup (1-3) and added 2 of our own (4-5):
1. Call free() with an address not obtained from malloc() - freeOne.C
2. Calling free() with an address not at the start of a chunk  - freeTwo.C
3. Calling free() a second time on the same pointer.  - freeThree.c
4. For leak testing, we called malloc() for 20 times into a int** array without freeing to ensure the correct amount of bytes being leaked and number of malloc()’d objects being leaked are as expected. - leakTester.C
5. malloc() more bytes than are available. - badMalloc.c

Correctness testing
To test our programs malloc() and free() outside of the error testing from section 2.1, we have 2 programs to check that we are both allocating the right amount of bytes (using 8 byte alignment) and freeing that memory. 

1. malloc() a struct for a linked list node of 20 bytes to ensure that alignment is correct and heading is added when malloc()’ing but only payload is returned - testMalloc.c
    1a. We used __attribute__((packed)); in order to ensure the struct stayed 20 bytes for testing because the struct was auto aligning to 24 bytes before malloc
2. malloc() 10 linked list nodes each with 4 integer fields and place them into an array, and after allocating, change the 4 integers for each object to ensure that changing those fields for one object did not change them for all, then free all allocations. - testingFree.c

Performance Testing (memgrind.c)

Our first 3 test cases were the provided test cases (1-3) and 2 of our design (4-5)
1. malloc() and immediately free() a 1-byte object, 120 times.
2. Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to dealloc
3. Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects.
4. Coalescing by malloc()’ing 1 byte 120 times into an array, freeing every other index and then doing the same for the remaining allocated objects
5. Alignment by mallocing 22 bytes 20 times to ensure that 24 bytes (excluding header) were actually being allocated (abiding by 8 byte alignment)


__________________________________________________________
Test descriptions/results


Error Testing

As explained in the assignment writeup, all 3 testcases provided should give an inappropriate pointer error, which they do.
1. free: Inappropriate pointer (freeOne.c:13)
2. free: Inappropriate pointer (freeTwo.c:13) mymalloc: 16 bytes leaked in 1 objects
3. free: Inappropriate pointer (freeThree.c:15)
Our last 2 testcases also reflect the expected results 
4. mymalloc: 320 bytes leaked in 20 objects
5. malloc: Unable to allocate 100008 bytes (badMalloc.c:12)

Correctness Testing

Output from testcase 1 to test alignment (Included print statements):
Size of struct LL node 20
Leak below shows that the 20 byte struct is aligned to 8 bytes (24 bytes) plus the 8 byte header, leading to 32 byte leak.
mymalloc: 32 bytes leaked in 1 objects

Output from testcase 2 where we ensured writing to objects didn't overwrite data from other objects and that our free worked:
Expected ID/Keys: 0 1 2 3   Actual ID/Keys:  0 1 2 3 
Expected ID/Keys: 1 2 3 4   Actual ID/Keys:  1 2 3 4 
Expected ID/Keys: 2 3 4 5   Actual ID/Keys:  2 3 4 5 
Expected ID/Keys: 3 4 5 6   Actual ID/Keys:  3 4 5 6 
Expected ID/Keys: 4 5 6 7   Actual ID/Keys:  4 5 6 7 
Expected ID/Keys: 5 6 7 8   Actual ID/Keys:  5 6 7 8 
Expected ID/Keys: 6 7 8 9   Actual ID/Keys:  6 7 8 9 
Expected ID/Keys: 7 8 9 10   Actual ID/Keys:  7 8 9 10 
Expected ID/Keys: 8 9 10 11   Actual ID/Keys:  8 9 10 11 
Expected ID/Keys: 9 10 11 12   Actual ID/Keys:  9 10 11 12 

Performance Testing (memgrind.c):
0.000000197 seconds 

__________________________________________________________
Design Notes

Our malloc traverses the heap array to find an open slot, and once we do, we allocate the item inside the heap array and reallocate for the free chunk
Our free traverses the heap array searching for the pointer to free and coalesces free chunks 

As aforementioned, we used __attribute__((packed)) in testingMalloc.c to prevent auto struct alignment to ensure our alignment works properly
