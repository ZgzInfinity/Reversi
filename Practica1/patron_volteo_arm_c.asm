@*********************************************************************************************
@ Fichero:	patron_volteo_arm_c.asm
@ Autores: 737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
@ Descrip:	Función patron_volteo_arm_c
@*********************************************************************************************

.text
.global patron_volteo_arm_c

@ al sumar dos chars, y ser uno -1, es necesario forzar que los chars sean signed para que al sumarse,
@ no queden bits de overflow (ej. 0xff + 0x02 = 0x101, deberia ser 0x01 ya que al pasar como parametro podria dar
@ problemas) opcion del compilador -fsigned-char

@ Parametros recibidos por registros
@ r0 = @tablero
@ r1 = @longitud
@ r2 = FA
@ r3 = CA
@ Parametros recibidos en pila y desapilados
@ r4 = SF
@ r5 = SC
@ r6 = color
@ Variables locales y longitud
@ r7 = posicion_valida
@ r8 = longitud
@ r9 = casilla (ficha_valida())

patron_volteo_arm_c:
		@ ip = @ donde estan las variables pasadas al invocar la funcion
		mov ip, sp
		PUSH {r4-r9, ip, lr}
		sub sp, sp, #4 @ se guarda espacio para posicion_valida
		ldmia ip, {r4, r5, r6} @ POP empezando en ip, de las variables pasadas al invocar la funcion, IA ya que
		@ queremos coger la variable en SP al llamar a patron_volteo y las 2 anteriores

		@ variables locales
		add r2, r2, r4	@ FA = FA + SF
		add r3, r3, r5	@ CA = CA + SC

		@ preparar llamada a ficha_valida
		PUSH {r0-r3}
		mov r1, r2 @ mover FA a r1
		mov r2, r3 @ mover CA a r2
		sub r3, ip, #36 @ *posicion_valida
		bl ficha_valida
		mov r9, r0 @ r9 = casilla (resultado de ficha_valida())
		POP {r0-r3, r7} @ r7 = posicion_valida

		ldr r8, [r1] @ r8 = longitud
		cmp r7, #1 @ if posicion_valida != 1, salto a else
		bne else
		cmp r9, r6 @ if casilla == color, se 'salta' a elseif
if:		addne r8, r8, #1 @ r8++
		strne r8, [r1] @ guardar longitud en memoria
		@ preparar llamda a patron_volteo (recursiva)
		PUSHne {r4-r6} @ guardamos r4-r6 parametros funcion
		blne patron_volteo_arm_c
		addne sp, sp, #12 @ 4 * 3 (subimos sp por el PUSH anterior)
		bne fin @ en r0 tenemos ya el valor a devolver
elseif: cmp r8, #0
		movgt r0, #1
		bgt fin
else:	mov r0, #0
fin:	POP {r4-r9, sp, pc}
