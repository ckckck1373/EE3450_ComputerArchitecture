#define MAX_SIZE 1024
.section .data
.align 8
# There are fours global variables:
#  * array: Store the input numbers
#  * low, mid, high: indices 
#  where array[low:mid] and array[mid+1:high] are sorted
# You can modify the following lines to test your program
# Also, max array length = 128 , that is 1024 byte. 
array: .dword  0, 4, 5, 7, 1, 2, 3
low: .dword 0
high: .dword 6
mid: .dword 3
temp: .space MAX_SIZE

.section .text
.global main
#  Mergesort(*testArray, first, last)
main:

    # assignn arguments and jump to sort
    la s0, array # 存每一個array的index進去a0
    la s1, temp  # temp[k]
    ld s2, high  # 6
    ld s3, low   # 0
    ld s4, mid   # 3
    add s5, s3, x0 # i = low
    add s7, x0, x0 # k = 0
    addi s6, s4, 1 # j = mid + 1
  
    # s0 array  
    # s1 temp
    # s2 high
    # s3 low
    # s4 mid
    # s5 i
    # s6 j
    # s7 k


    Loop: 
        blt s4, s5, Exit # i <= mid
        blt s2, s6, Exit # j <= high

        slli t0, s5, 3   # t0 = i * 16 // i
        slli t1, s6, 3   # t1 = j * 16 // j
        slli t5, s7, 3  # t5 = k * 16 // k

        add t0, t0, s0   # t0 = t0 + s0 // i
        add t1, t1, s0   # t1 = t1 + s0 // j

        ld t2, 0(t0)    # t2 = a[t0]  // a[i]
        ld t3, 0(t1)    # t3 = a[t1]  // a[j]

        bge t2, t3, Else  # if(a[i]>=a[j]) go to Else

        # ifblock 
        add t5, t5, s1 # t5 = t5 + s1 // k
        ld t4, 0(t0) # a[i]
        sd t4, 0(t5) # temp[k] = a[i]
        addi s7, s7, 1 # k++
        addi s5, s5, 1 # i++ 
        beq zero, zero, Loop # while again

      Else: # else
        add t5, t5, s1 # t5= t5 + s1  // k
        ld t4, 0(t1)  # a[j]
        sd t4, 0(t5) # temp[k] = a[j]
        addi s7, s7, 1 # k++
        addi s6, s6, 1 # j++
        beq zero, zero, Loop # while again
   
    Exit: 

  # Insert all the remaining values from i to mid into temp[]
  Loop2: 
      blt s4, s5, Exit2 # i <= mid

      slli t0, s5, 3   # t0 = i * 16 // i
      slli t5, s7, 3   # t5 = k * 16 // k

      add t0, t0, s0   # t0 = t0 + s0
      add t5, t5, s1   # t5 = t5 + s1

      ld t2, 0(t0) # t2 = a[t0] // a[i]
      sd t2, 0(t5) # temp[k] = a[i]
      addi s7, s7, 1 # k++
      addi s5, s5, 1 # i++
      beq zero, zero, Loop2 # while again
  Exit2:
    
  # Insert all the remaining values from j to high into temp[]
  Loop3:
    blt s2, s6, Exit3 # j <= high

    slli t1, s6, 3 # t1 = j * 16 // j
    slli t5, s7, 3 # t5 = k * 16 // k

    add t1, t1, s0 # t1 = t1 + s0
    add t5, t5, s1 # t5 = t5 + s1

    ld t2, 0(t1) # t2 = a[t1] // a[j]
    sd t2, 0(t5) # temp[k] = a[j]
    addi s7, s7, 1 # k++
    addi s6, s6, 1 # j++
    beq zero, zero, Loop3 # while again 
  Exit3:

  # Assign sorted data stored in temp[] to a[]
  ld s5, low      # i = low
  For:
    blt s2, s5, Exit4  # i < high
    sub t1, s5, s3  # t1 = i - low 
    slli t0, s5, 3  # t0 = i * 16 // i
    slli t1, t1, 3  # t3 = (i - low) * 16
    add t1, t1, s1 # t1 = t1 + s1
    add t0, t0, s0 # t0 = t0 + s0
    
    ld t2, 0(t1)   # t2 = temp[i-low]
    sd t2, 0(t0)   # a[i] = temp[i-low]
    addi s5, s5, 1  # i++
    beq zero, zero, For
  Exit4:

  # do not modify following
  li  a0, 0      # assign 0 to a0
  ret            # return (jump to the addr store in register ra)




