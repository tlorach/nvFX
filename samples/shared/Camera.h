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
// Camera
//-----------------------------------------------------------------------------
struct Camera
{
    vec3    curEyePos, curFocusPos, curObjectPos;
    vec3    eyePos, focusPos, objectPos;
    float   tau;
    float   epsilon;
    Camera()
    {
        epsilon = 0.001f;
        tau = 0.2f;
        curEyePos = vec3(0.7f,0.7f,0.7f);
        curFocusPos = vec3(0,0,0);
        curObjectPos = vec3(0,0,0);
        eyePos = vec3(0.7f,0.7f,0.7f);
        focusPos = vec3(0,0,0);
        objectPos = vec3(0,0,0);
    }
    void rotateH(float s, bool bPan=false)
    {
        vec3 p = eyePos;
        vec3 o = focusPos;
        vec3 po = p-o;
        float l = po.norm();
        vec3 dv;
        cross(dv, po, vec3(0,1,0) );
        dv *= s;
        p += dv;
        po = p-o;
        float l2 = po.norm();
        l = l2 - l;
        p -= (l/l2) * (po);
        eyePos = p;
        if(bPan)
            focusPos += dv;
    }
    void rotateV(float s, bool bPan=false)
    {
        vec3 p = eyePos;
        vec3 o = focusPos;
        vec3 po = p-o;
        float l = po.norm();
        vec3 dv;
        cross(dv, po, vec3(0,-1,0) );
        dv.normalize();
        vec3 dv2;
        cross(dv2, po, dv );
        dv2 *= s;
        p += dv2;
        po = p-o;
        float l2 = po.norm();
        l = l2 - l;
        p -= (l/l2) * (po);
        eyePos = p;
        if(bPan)
            focusPos += dv2;
    }
    void move(float s, bool bPan)
    {
        vec3 p = eyePos;
        vec3 o = focusPos;
        vec3 po = p-o;
        po *= s;
        p -= po;
        if(bPan)
            focusPos -= po;
        eyePos = p;
    }
    bool update(float dt)
    {
        bool bContinue = false;
        static vec3 eyeVel = vec3(0,0,0);
        static vec3 eyeAcc = vec3(0,0,0);
        if(nv_norm(curEyePos - eyePos) > epsilon)
        {
            bContinue = true;
            vec3 dV = curEyePos - eyePos;
            eyeAcc = (-2.0f/tau)*eyeVel - dV/(tau*tau);
            // integrate
            eyeVel += eyeAcc * vec3(dt,dt,dt);
            curEyePos += eyeVel * vec3(dt,dt,dt);
        } else {
            eyeVel = vec3(0,0,0);
            eyeAcc = vec3(0,0,0);
        }

        static vec3 focusVel = vec3(0,0,0);
        static vec3 focusAcc = vec3(0,0,0);
        if(nv_norm(curFocusPos - focusPos) > epsilon)
        {
            bContinue = true;
            vec3 dV = curFocusPos - focusPos;
            focusAcc = (-2.0f/tau)*focusVel - dV/(tau*tau);
            // integrate
            focusVel += focusAcc * vec3(dt,dt,dt);
            curFocusPos += focusVel * vec3(dt,dt,dt);
        } else {
            focusVel = vec3(0,0,0);
            focusAcc = vec3(0,0,0);
        }

        static vec3 objectVel = vec3(0,0,0);
        static vec3 objectAcc = vec3(0,0,0);
        if(nv_norm(curObjectPos - objectPos) > epsilon)
        {
            bContinue = true;
            vec3 dV = curObjectPos - objectPos;
            objectAcc = (-2.0f/tau)*objectVel - dV/(tau*tau);
            // integrate
            objectVel += objectAcc * vec3(dt,dt,dt);
            curObjectPos += objectVel * vec3(dt,dt,dt);
        } else {
            objectVel = vec3(0,0,0);
            objectAcc = vec3(0,0,0);
        }
        return bContinue;
    }
};
