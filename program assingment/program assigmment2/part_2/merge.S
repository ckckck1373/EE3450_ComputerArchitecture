########################################
# Make 'merge' a function: 
#   Similar to the previous task, the only difference is merge now becomes a function.
#   That is,
#   a0 --> nums[]
#   a1 --> low
#   a2 --> mid 
#   a3 --> high 
#   as designated in declaraction of merge() in main.c
#   Again, [low, mid] sorted, [mid+1, high] sorted
########################################


#define MAX_SIZE 1024
.section .data
.align 8
temp: .space MAX_SIZE 


.section .text
.global merge 
merge:

  # your code goes here
  # remember to save register such as s0 or ra onto the stack if you used it in the assembly. 

  la a4, temp # temp
  addi t5, a2, 1 # s1 = j = mid + 1

  # a0 array
  # a1 low 
  # a2 mid
  # a3 high
  # s3 temp # delete
  # s0 i # delete
  # s1 j # delete
  # s2 k ＃ delete
  
  # t0 i*
  # t1 j*
  # t2 k*
  # t3 a[i*]
  # t4 a[j*]
  # t5 temp[k*]
  # t6 a[i**]

  # a4 temp 
  # a5 mid*8 + array
  # a6 high*8 + array
  # a7 a[j**]


  slli t0, a2, 3  # t0 = mid * 8 //mid*
  add t0, t0, a0 # t0 = a0 + t0

  ld t1, 0(t0) # t0 = a[mid]
  ld t2, 8(t0) # t1 = a[mid+1]
  bge t2, t1, Exit4  # if we don't need to sort, then end in advance
 


  slli t0, a1, 3    # t0 = i * 8 //i*
  slli t1, t5, 3    # t1 = j * 8 //j*
  slli t2, zero, 3   # t2 = k * 8 //k*

  slli a5, a2, 3     # mid * 8 // mid*
  add  a5, a5, a0    # mid * 8 + array 
  slli a6, a3, 3     # high * 8 // high*
  add a6, a6, a0     # high * 8 // array

  add t3, t0, a0    # t3 = t0 + a0 // i* + a0
  add t4, t1, a0    # t4 = t1 + a0 // j* + a0
  add t5, t2, a4    # t5 = t2 + s3 // k* + a4


    #  blt a2, t6, Exit  # i <= mid
      blt a6, t1, Exit  # j* <= high*


      ld t6, 0(t3)   # t3 = a[t3] // a[i**]
      ld a7, 0(t4)   # t4 = a[t4] // a[j**]

    Loop: 
      bge t6, a7, Else  # if(a[i**]>=a[j**]) go to Else

      # ifblock
      sd t6, 0(t5) # temp[k*] = a[i**]

      addi t5, t5, 8 # a[k*]++
      addi t3, t3, 8 # a[i*]++
      ld t6, 0(t3)   # t3 = a[t3] // a[i**]
      blt a5, t3, Exit3  # i* > mid*         
      beq zero, zero, Loop # while again
    
    Else: # else
      sd a7, 0(t5) # temp[k*] = a[j**]

      addi t5, t5, 8  # a[k*]++
      addi t4, t4, 8  # a[j*]++
      ld a7, 0(t4)   # t4 = a[t4] // a[j*]
      blt a6, t4, Loop2  # j <= high
      beq zero, zero, Loop # while again

  Exit: 
    
# Insert all the remaining values from i to mid into temp[]
Loop2: 
    blt a5, t3, Exit3 # a[i*] <= mid*       

    ld t6, 0(t3) # t6 = a[i*] // a[i**]
    sd t6, 0(t5) # temp[k*] = a[i**]

    addi t5, t5, 8 # a[k*]++
    addi t3, t3, 8 # a[i*]++  
    beq zero, zero, Loop2 # while again
Exit2:


#  Insert all the remaining values from j to high into temp[]
Loop3: 
   blt a6, t4, Exit3 # a[j*] <= high* 
 
   ld a7, 0(t4) # a7 = a[t4] // a[j**]
   sd a7, 0(t5) # temp[k] = a[j**]

   addi t5, t5, 8 # a[k*]++
   addi t4, t4, 8 # a[j*]++
   beq zero, zero, Loop3 # while again
Exit3: 

# Assign sorted data stored in temp[] to a[]


addi t5, t5, -8 # temp[k*]--
addi t4, t4, -8 # a[j*]--

For:
  blt t5, a4, Exit4  # array > a[j*] go to Exit4

  ld a7, 0(t5)  # a7 = temp[k*]
  sd a7, 0(t4)  # a[j*] = a7
  addi t5, t5, -8 # temp[k*]--
  addi t4, t4, -8 # a[j*]--
  beq zero, zero, For
Exit4: 


  ret      # return, same as "jr ra"


