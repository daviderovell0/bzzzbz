package uk.me.berndporr.kiss_fft;

import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import org.apache.commons.math3.complex.Complex;
import org.apache.commons.math3.transform.DftNormalization;
import org.apache.commons.math3.transform.FastFourierTransformer;
import org.apache.commons.math3.transform.TransformType;
import org.junit.Test;
import org.junit.runner.RunWith;

import static junit.framework.Assert.assertFalse;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class BenchTest {


    private void doFFTandIFFTCommons() {
        FastFourierTransformer fastFourierTransformer = new FastFourierTransformer(DftNormalization.STANDARD);

        final int testsize = 65536;
        Complex[] indata = new Complex[testsize];
        for (int i = 0; i < testsize; i++) indata[i] = new Complex(Math.random() - 0.5);
        Log.d("FFTTest", "start FFTCommons");
        for(int i=0;i<20;i++) {
            Log.d("FFTTest", ""+i);
            Complex[] indata2 = fastFourierTransformer.transform(indata, TransformType.FORWARD);
            indata = fastFourierTransformer.transform(indata2, TransformType.INVERSE);
        }
        Log.d("FFTTest", "stop FFTCommons");
    }


    private void doFFTandIFFTKISS() {
        KISSFastFourierTransformer kissfastFourierTransformer = new KISSFastFourierTransformer();

        final int testsize = 65536;
        Complex[] indata = new Complex[testsize];
        for (int i = 0; i < testsize; i++) indata[i] = new Complex(Math.random() - 0.5);
        Log.d("FFTTest", "start FFT KISS");
        for(int i=0;i<20;i++) {
            Log.d("FFTTest", ""+i);
            Complex[] indata2 = kissfastFourierTransformer.transform(indata,TransformType.FORWARD);
            indata = kissfastFourierTransformer.transform(indata2,TransformType.INVERSE);
        }
        Log.d("FFTTest", "stop FFT KISS");
    }


    @Test
    public void doTimingTest() throws Exception {
        doFFTandIFFTKISS();
        doFFTandIFFTCommons();
    }




}


