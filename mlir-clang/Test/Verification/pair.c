// RUN: mlir-clang %s --function=create | FileCheck %s

typedef struct {
  int a, b;
} pair;

pair byval(pair a, int x) {
  a.b = x;
  return a;
}

int create() {
  pair p;
  p.a = 0;
  p.b = 1;
  pair p2 = byval(p, 2);
  return p2.a;
}

// CHECK:   func @create() -> i32 {
// CHECK-NEXT:     %c2_i32 = constant 2 : i32
// CHECK-NEXT:     %c1_i32 = constant 1 : i32
// CHECK-NEXT:     %c0_i32 = constant 0 : i32
// CHECK-NEXT:     %0 = memref.alloca() : memref<1x2xi32>
// CHECK-NEXT:     %1 = memref.alloca() : memref<1x2xi32>
// CHECK-NEXT:     affine.store %c0_i32, %1[0, 0] : memref<1x2xi32>
// CHECK-NEXT:     affine.store %c1_i32, %1[0, 1] : memref<1x2xi32>
// CHECK-NEXT:     %2 = memref.cast %1 : memref<1x2xi32> to memref<?x2xi32>
// CHECK-NEXT:     %3 = memref.cast %0 : memref<1x2xi32> to memref<?x2xi32>
// CHECK-NEXT:     call @byval(%2, %c2_i32, %3) : (memref<?x2xi32>, i32, memref<?x2xi32>) -> ()
// CHECK-NEXT:     %4 = affine.load %0[0, 0] : memref<1x2xi32>
// CHECK-NEXT:     return %4 : i32
// CHECK-NEXT:   }
// CHECK:   func @byval(%arg0: memref<?x2xi32>, %arg1: i32, %arg2: memref<?x2xi32>) {
// CHECK-NEXT:     affine.store %arg1, %arg0[0, 1] : memref<?x2xi32>
// CHECK-NEXT:     %0 = affine.load %arg0[0, 0] : memref<?x2xi32>
// CHECK-NEXT:     affine.store %0, %arg2[0, 0] : memref<?x2xi32>
// CHECK-NEXT:     %1 = affine.load %arg0[0, 1] : memref<?x2xi32>
// CHECK-NEXT:     affine.store %1, %arg2[0, 1] : memref<?x2xi32>
// CHECK-NEXT:     return
// CHECK-NEXT:   }
