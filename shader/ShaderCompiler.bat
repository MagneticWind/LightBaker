fxc /T vs_5_0 /E VS /Fo ..\data\shader\norm.v norm.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\norm.p norm.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\norm_sr.v norm_sr.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\norm_sr.p norm_sr.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\sky.v sky.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\sky.p sky.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\tonemapping.v tonemapping.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\tonemapping.p tonemapping.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\depth_pn.v depth_pn.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\csm_pn.v csm_pn.hlsl /Od /Zi

fxc /T vs_5_0 /E VS /Fo ..\data\shader\ssao.v ssao.hlsl /Od /Zi
fxc /T ps_5_0 /E PS /Fo ..\data\shader\ssao.p ssao.hlsl /Od /Zi

fxc /T cs_5_0 /E CS /Fo ..\data\shader\reduceto1d.c reduceto1d.hlsl /Od /Zi
fxc /T cs_5_0 /E CS /Fo ..\data\shader\reduceto1pixel.c reduceto1pixel.hlsl /Od /Zi