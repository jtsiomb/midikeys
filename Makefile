obj = main.obj midi.obj keyb.obj
bin = midikeys.exe

CC = wcc
LD = wlink
CFLAGS = -d3 -s -zq -bt=dos

$(bin): $(obj)
	%write objects.lnk $(obj)
	$(LD) debug all name $@ system dos file { @objects } $(LDFLAGS)

.c.obj:
	$(CC) -fo=$@ $(CFLAGS) $<

.asm.obj:
	nasm -f obj -o $@ $(ASFLAGS) $<

clean: .symbolic
	del *.obj
	del $(bin)
