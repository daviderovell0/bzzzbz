package uk.me.berndporr.kiss_fft;

import android.support.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static junit.framework.Assert.assertFalse;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class LoadTest {

    @Test
    public void useAppContext() throws Exception {
        KISSFastFourierTransformer kissFastFourierTransformer = new KISSFastFourierTransformer();
        assertFalse(kissFastFourierTransformer == null);
    }
}
