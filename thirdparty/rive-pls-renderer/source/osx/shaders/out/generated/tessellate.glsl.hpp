#pragma once

#include "tessellate.exports.h"

namespace rive {
namespace pls {
namespace glsl {
const char tessellate[] = R"===(#define ra 10
#ifdef W
T0(P)q0(0,g,HC);q0(1,g,IC);q0(2,g,AC);q0(3,T,OB);U0
#endif
z1 k0 I(0,g,B3);k0 I(1,g,C3);k0 I(2,g,z2);k0 I(3,C1,d3);c6 I(4,uint,k4);A1 p A g8(d S0,d v0,d I0,d d1){A t;t[0]=(any(notEqual(S0,v0))?v0:any(notEqual(v0,I0))?I0:d1)-S0;t[1]=d1-(any(notEqual(d1,I0))?I0:any(notEqual(I0,v0))?v0:S0);return t;}
#ifdef W
N1 O1 U1 Q2(r7,z9,TB);Q2(v7,A9,GC);V1 float h8(d m,d b){float sa=dot(m,b);float i8=dot(m,m)*dot(b,b);return(i8==.0)?1.:clamp(sa*inversesqrt(i8),-1.,1.);}e1(WD,P,r,j,L){w0(L,r,HC,g);w0(L,r,IC,g);w0(L,r,AC,g);w0(L,r,OB,T);Q(B3,g);Q(C3,g);Q(z2,g);Q(d3,C1);Q(k4,uint);d S0=HC.xy;d v0=HC.zw;d I0=IC.xy;d d1=IC.zw;bool j8=j<4;float y=j8?AC.z:AC.w;int k8=int(j8?OB.x:OB.y);float l6=float(k8<<16>>16);float W4=float(k8>>16);d h1=d((j&1)==0?l6:W4,(j&2)==0?y+1.:y);uint r1=OB.z&0x3ffu;uint l8=(OB.z>>10)&0x3ffu;uint e3=OB.z>>20;uint D=OB.w;if(W4<l6){D|=H4;}if((W4-l6)*v.E5<.0){h1.y=2.*y+1.-h1.y;}if((D&o9)!=0u){uint ta=z0(GC,X6(D)).z;A m8=V0(uintBitsToFloat(z0(TB,ta*2u)));d n8=h0(m8,-2.*v0+I0+S0);d o8=h0(m8,-2.*I0+d1+v0);float c1=max(dot(n8,n8),dot(o8,o8));float D2=max(ceil(sqrt(.75*4.*sqrt(c1))),1.);r1=min(uint(D2),r1);}uint X4=r1+l8+e3-1u;A T1=g8(S0,v0,I0,d1);float o1=acos(h8(T1[0],T1[1]));float f2=o1/float(l8);float m6=determinant(A(I0-S0,d1-v0));if(m6==.0)m6=determinant(T1);if(m6<.0)f2=-f2;B3=g(S0,v0);C3=g(I0,d1);z2=g(float(X4)-abs(W4-h1.x),float(X4),(e3<<10)|r1,f2);if(e3>1u){A n6=A(T1[1],AC.xy);float ua=acos(h8(n6[0],n6[1]));float p8=float(e3);if((D&(Y3|G4))==G4){p8-=2.;}float o6=ua/p8;if(determinant(n6)<.0)o6=-o6;d3.xy=AC.xy;d3.z=o6;}k4=D;g B;B.x=h1.x*(2./n9)-1.;B.y=h1.y*v.E5-sign(v.E5);B.zw=d(0,1);S(B3);S(C3);S(z2);S(d3);S(k4);f1(B);}
#endif
#ifdef HB
q2(T,XD){N(B3,g);N(C3,g);N(z2,g);N(d3,C1);N(k4,uint);d S0=B3.xy;d v0=B3.zw;d I0=C3.xy;d d1=C3.zw;A T1=g8(S0,v0,I0,d1);float va=max(floor(z2.x),.0);float X4=z2.y;uint q8=uint(z2.z);float r1=float(q8&0x3ffu);float e3=float(q8>>10);float f2=z2.w;uint D=k4;float f3=X4-e3;float v1=va;if(v1<=f3){D&=~Y3;}else{S0=v0=I0=d1;T1=A(T1[1],d3.xy);r1=1.;v1-=f3;f3=e3;if((D&Y3)!=0u){if(v1<2.5)D|=K5;if(v1>1.5&&v1<3.5)D|=l7;}else if((D&G4)!=0u){f3-=2.;v1--;}f2=d3.z;D|=f2<.0?I4:m7;}d Y4;float o1=.0;if(v1==.0||v1==f3||(D&Y3)!=0u){bool Q4=v1<f3*.5;Y4=Q4?S0:d1;o1=Z6(Q4?T1[0]:T1[1]);}else if((D&k7)!=0u){Y4=v0;}else{float S1,g3;if(r1==f3){S1=v1/r1;g3=.0;}else{d e0,g0,S4=v0-S0;d Y7=d1-S0;d r8=I0-v0;g0=r8-S4;e0=-3.*r8+Y7;d wa=g0*(r1*2.);d xa=S4*(r1*r1);float Z4=.0;float ya=min(r1-1.,v1);d p6=normalize(T1[0]);float za=-abs(f2);float Aa=(1.+v1)*abs(f2);for(int F3=ra-1;F3>=0;--F3){float l4=Z4+exp2(float(F3));if(l4<=ya){d q6=l4*e0+wa;q6=l4*q6+xa;float Ba=dot(normalize(q6),p6);float r6=l4*za+Aa;r6=min(r6,D4);if(Ba>=cos(r6))Z4=l4;}}float Ca=Z4/r1;float v8=v1-Z4;float a5=acos(clamp(p6.x,-1.,1.));a5=p6.y>=.0?a5:-a5;o1=v8*f2+a5;d U2=d(sin(o1),-cos(o1));float m=dot(U2,e0),c5=dot(U2,g0),f0=dot(U2,S4);float Da=max(c5*c5-m*f0,.0);float A2=sqrt(Da);if(c5>.0)A2=-A2;A2-=c5;float w8=-.5*A2*m;d v6=(abs(A2*A2+w8)<abs(m*f0+w8))?d(A2,m):d(f0,A2);g3=(v6.y!=.0)?v6.x/v6.y:.0;g3=clamp(g3,.0,1.);if(v8==.0)g3=.0;S1=max(Ca,g3);}d Ea=q3(S0,v0,S1);d x8=q3(v0,I0,S1);d Fa=q3(I0,d1,S1);d y8=q3(Ea,x8,S1);d z8=q3(x8,Fa,S1);Y4=q3(y8,z8,S1);if(S1!=g3)o1=Z6(z8-y8);}r2(T(floatBitsToUint(C1(Y4,o1)),D));}
#endif
)===";
} // namespace glsl
} // namespace pls
} // namespace rive