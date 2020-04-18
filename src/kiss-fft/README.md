# KISS FFT

KISS FFT - A mixed-radix Fast Fourier Transform in C with an Android
JNI wrapper.

## In C

### Complex to Complex FFT and IFFT

The basic usage for 1-d complex FFT is:

```
#include "kiss_fft.h"

kiss_fft_cfg cfg = kiss_fft_alloc( nfft, is_inverse_fft, 0, 0 );
kiss_fft_cpx *cx_in = new kiss_fft_cpx[nfft];
kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft];
    
// put kth sample in cx_in[k].r and cx_in[k].i
            
kiss_fft( cfg , cx_in , cx_out );
            
// transformed. DC is in cx_out[0].r and cx_out[0].i 
            
free(cfg);
delete[] cx_in;
delete[] cx_out;
```
* `nfft` is the number of samples in both time- and frequency domain
* `is_inverse` is 0 for the forward transform and 1 for the inverse
* `cx_in` and `cx_out` are arrays of `nfft` samples
* Frequency domain: `cx_out[0]` is the DC bin of the FFT and `cx_out[nfft/2]`
is the Nyquist bin (if exists).
* Files: `kiss_fft.h`, `kiss_fft.c` and `_kiss_fft_guts.h`.

### Real valued FFT

A real valued optimized FFT which takes real valued signals as its input is implemtned in `kiss_fftr.h` and `kiss_fftr.c`. It returns the positive half-spectrum: (nfft/2+1) complex frequency bins.

#### Real signal to complex frequencies transform

```
#include "kiss_fftr.h"

kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
kiss_fft_scalar *cx_in = new kiss_fft_scalar[nfft];
kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft/2+1];

// put `nfft` samples in cx_in[k]

kiss_fftr(cfg, cx_in, cx_out);

// Process the spectrum `cx_out` here: We have `nfft/2+1` (!) samples.
            
free(cfg);
delete[] cx_in;
delete[] cx_out;
```

#### Complex frequencies to real signal transform

```
#include "kiss_fftr.h"

kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 1, 0, 0);
kiss_fft_cpx *cx_in = new kiss_fft_cpx[nfft/2+1];
kiss_fft_scalar *cx_out = new kiss_fft_scalar[nfft];

// put kth frequency sample in cx_in[k] up to index `nfft/2`.
// No need to populate the mirror.

kiss_fftr(cfg, cx_in, cx_out);

// Process signal `cx_out` here. It has again `nfft` samples
// and is real valued.
            
free(cfg);
delete[] cx_in;
delete[] cx_out;
```

### Setting the data type for the FFT
(Note this is only for the plain C version. The Android wrapper always expects double. If you use JNI then leave it to the default type.)
The default data type is `double` which is kept in the C macro `kiss_fft_scalar` but with a compiler-define this can be changed to (for example in CMake):
* float: `add_definitions(-Dkiss_fft_scalar=float)` where the C macro `kiss_fft_scalar` is defined as `float`.
* 16 bit int: `add_definitions(-DFIXED_POINT=16)` where the C macro `FIXED_POINT` is set to the number 16 which in turn then sets `kiss_fft_scalar` to `int16_t`.
* 32 bit int: `add_definitions(-DFIXED_POINT=32)` where the C macro `FIXED_POINT` is set to the number 32 which in turn then sets `kiss_fft_scalar` to `int32_t`.

### Installation / Usage
The library is so small that you can directly include the sources in your
project or you can pre-package it as a static library and then link it
into your project. Create the static library (with the help of cmake):
```
cmake . -DCMAKE_BUILD_TYPE=RelWithDebInfo
make
make install
```
which is installed in the usual places (e.g. `/usr/local/lib` and
`/usr/local/include`) and is called `libkiss-fft.a`.

## Android
Super-fast native FFTs under Android. Only double (default) for the FFT is supported.

### Compilation
Open this project in Android studio and run "Build". Depending
on the config you'll generate a debug version of the kiss-fft
library or a release version.

### Installation
The Android library is in `jnifft/build/outputs/`.
Just import it into your Android project with "New-Module-Android Library" and add
the dependency with
```
compile project(":jnifft-release")
```

### Complex to Complex transform
```
kissFastFourierTransformer = new KISSFastFourierTransformer();
Complex[] outdata = kissFastFourierTransformer.transform(indata, TransformType.FORWARD);
```
which transforms from Complex to Complex as defined in the apache Commons.
The constant `TransformType` is also defined in apache Commons which determines
if it's a forward or inverse transform. It can be used as a direct
replacement of the apache commons FFT function.

There are also convenience functions as implemented in the apache commons library for double and Double which perform the conversion to Complex in C++. The function for the primitive type double is slightly faster than the one with Double.

### Real to Complex and Complex to Real transform
For real valued sequences there are two optimised functions which
directly perform the FFT on the raw `double` buffer without any
conversion to `Complex`. For such real valued sequences this runs at least
twice as fast as the Complex FFT functions above.
The complex frequency sequence of the real sequence of length N has the length
N/2+1 and then expands back to length N by the inverse transform:
```
public Complex[] transformRealOptimisedForward(double[] v)
public double[] transformRealOptimisedInverse(Complex[] v)
```

### Unit tests
Run `FFTTest` and `FFTRTest` which compare the results to that from the
apache commons FFT functions and perform an ifft(fft) test to
check for rounding errors.

## Attribution
This is a fork, stripped down and further debugged version of the original kiss-fft library
by Mark Borgerding <Mark@Borgerding.net>.
