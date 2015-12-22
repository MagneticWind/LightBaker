fxc /T vs_5_0 /E VS /Fo ..\data\shader\norm.v norm.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\norm.p norm.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\sky.v sky.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\sky.p sky.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\tonemapping.v tonemapping.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\tonemapping.p tonemapping.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\depth_pn.v depth_pn.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\ssao.v ssao.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\ssao.p ssao.hlsl /Od /Zi