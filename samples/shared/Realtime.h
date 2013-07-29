/*
    Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of NVIDIA CORPORATION nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Please direct any questions to tlorach@nvidia.com (Tristan Lorach)
*/
//-----------------------------------------------------------------------------
// Realtime work
//-----------------------------------------------------------------------------
struct Realtime
{
    bool                bNonStopRendering;
    int                 renderCnt;
#ifdef WIN32
    LARGE_INTEGER       start_time, end_time, freq;
#endif
    int                 timing_counter;
    int                 maxTimeSamples;
    double              frameDT;
    Realtime()
    {
        bNonStopRendering = true;
        renderCnt = 1;
        timing_counter = 0;
        maxTimeSamples = 4;
        frameDT = 1.0/60.0;
#ifdef WIN32
        QueryPerformanceCounter(&start_time);
        QueryPerformanceCounter(&end_time);
#endif
    }
    bool update(bool bContinueToRender)
    {
#ifdef WIN32
        bool updated = false;
        int totalSamples;
        totalSamples = (bContinueToRender || bNonStopRendering) ? maxTimeSamples : timing_counter;

        if((timing_counter >= totalSamples) && (totalSamples > 0))
        {
            timing_counter = 0;
            QueryPerformanceCounter(&end_time);
            QueryPerformanceFrequency(&freq);
            frameDT = (((double)end_time.QuadPart) - ((double)start_time.QuadPart))/((double)freq.QuadPart);
            frameDT /= totalSamples;
#ifdef USESVCUI
            if(g_pWinHandler)
            {
                IControlLabel* pLabel = g_pWinHandler->GetLabel("FPS");
                if(pLabel) pLabel->SetString("total Time = %f ms; %d fps (for %d samples)\n", frameDT, (int)(1.0/frameDT), totalSamples);
            }
#endif
            // setup some limits for really slow cases
            if(frameDT > (1.0/60.0) )
                frameDT = (1.0/60.0);
            // update the amount of samples to average, depending on the speed of the scene
            maxTimeSamples = (int)(0.5/(frameDT));
            if(maxTimeSamples == 0)
                maxTimeSamples = 10; // just to avoid 0...
            updated = true;
        }
        if(bContinueToRender || bNonStopRendering)
        {
            if(timing_counter == 0)
                QueryPerformanceCounter(&start_time);
            timing_counter++;
        }
        return updated;
#else
	// Linux/OSX etc. TODO
	return true;
#endif
    }
};
