# spx2wav
微信高清语音 spx 转 wav 解决方案, 基于 speex-devel 和微信官方提供的 declib, 修改了部分错误，重写了 Makefile，在 Debian & CentOS 下测试通过.

#依赖
speex-devel 或 libspeex.
lame

#编译方法
yum instal speex-devel 
cd spx2wav
make
cp spx2dev /usr/local/bin

#使用
spx2dev voice.speex voice.wav

#结合 lame 使用
/usr/local/bin/spx2dev voice.speex voice.wav && /usr/local/bin/lame -S -V 9 voice.wav voice.mp3 && rm -f voice.wav

