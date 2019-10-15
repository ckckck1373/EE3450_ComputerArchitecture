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
  addi sp, sp, -40
  sd ra, 32(sp)  # return address
  sd s3, 24(sp)  # temp
  sd s2, 16(sp)  # k
  sd s1, 8(sp)   # j
  sd s0, 0(sp)   # i
   
  # 是跳8嗎? 64bits?

 
  la s3, temp # temp
  add s0, a1, zero # s0 = i
  addi s1, a2, 1 # s1 = j
  add s2, zero, zero # s2 = k

  # a0 array
  # a1 low 
  # a2 mid
  # a3 high
  # s3 temp
  # s0 i
  # s1 j
  # s2 k

  Loop: 
      blt a2, s0, Exit  # i <= mid
      blt a3, s1, Exit  # j <= high

      slli t3, s0, 3    # t3 = i * 8 //i
      slli t4, s1, 3    # t4 = j * 8 //j
      slli t5, s2, 3    # t5 = k * 8 //k

      add t3, t3, a0    # t3 = t3 + s0 // i
      add t4, t4, a0    # t4 = t4 + s0 // j

      ld t3, 0(t3)   # t3 = a[t3] // a[i]
      ld t4, 0(t4)   # t4 = a[t4] // a[j]

      bge t3, t4, Else  # if(a[i]>=a[j]) go to Else


      # ifblock
      add t5, t5, s3 # t5 = t5 + s3 // k
      sd t3, 0(t5) # temp[k] = a[i]
      addi s2, s2, 1 # k++
      addi s0, s0, 1 # i++
      beq zero, zero, Loop # while again
    
    Else: # else
      add t5, t5, s3 # t5 = t5 + s3 //k
      sd t4, 0(t5) # a[j]
      addi s2, s2, 1 # k++
      addi s1, s1, 1 # j++
      beq zero, zero, Loop # while again

  Exit: 

# Insert all the remaining values from i to mid into temp[]
Loop2: 
    blt a2, s0, Exit2 # i <= mid

    slli t3, s0, 3 # t3 = i * 8 // i
    slli t5, s2, 3 # t4 = k * 8 // k

    add t3, t3, a0 # t3 = t3 + s3
    add t5, t5, s3 # t5 = t5 + s4

    ld t3, 0(t3) # t3 = a[t3] // a[i]
    sd t3, 0(t5) # temp[k] = a[i]
    addi s2, s2, 1 # k++
    addi s0, s0, 1 # i++
    beq zero, zero, Loop2 # while again
Exit2:


# Insert all the remaining values from j to high into temp[]
Loop3: 
  blt a3, s1, Exit3 # j <= high 

  slli t4, s1, 3 # t4 = j * 8 // j
  slli t5, s2, 3 # t5 = k * 8 // k

  add t4, t4, a0 # t4 = t4 + array
  add t5, t5, s3 # t5 = t5 + temp

  ld t4, 0(t4) # t4 = a[t4] // a[j]
  sd t4, 0(t5) # temp[k] = a[j]

  addi s2, s2, 1 # k++
  addi s1, s1, 1 # j++
  beq zero, zero, Loop3 # while again
Exit3: 

# Assign sorted data stored in temp[] to a[]
add s0, a1, zero # i = low ********

For:
  blt a3, s0, Exit4  # i < high
  sub t3, s0, a1 # t3 = i - low 
  slli t4, s0, 3  # t4 = i * 8 // i
  slli t3, t3, 3  # t3 = {i - low} * 8
  add t3, t3, s3 # t3 = t3 + s3
  add t4, t4, a0 # t0 = t0 + s0


  ld t3, 0(t3)  # t3 = temp[i-low]
  sd t3, 0(t4)  # a[i] = temp[i-low]
  addi s0, s0, 1 # i++
  beq zero, zero, For
Exit4: 


    ld  s0, 0(sp)
    ld  s1, 8(sp)
    ld  s2, 16(sp)
    ld  s3, 24(sp)
    ld  ra, 32(sp)
    addi sp, sp, 40

  ret      # return, same as "jr ra"

