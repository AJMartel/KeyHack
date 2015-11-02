#ifndef		KEYHACK_H
# define	KEYHACK_H

// Permet de tester sur des boards ne supportant pas la librairie
# if !defined( __AVR_ATmega32U4__ )
class	SimulKeyboard
{
	public:
		void	begin( void )
		{
			Serial.println();
			Serial.println( "BEGIN" );
			return ;
		}
		void	end( void )
		{
			Serial.println( "END" );
			Serial.println();
			return ;
		}
		size_t	press( uint8_t k )
		{
			Serial.print( "PRESS " );
			Serial.println( k );
			return ( 1 );
		}
		size_t	write( uint8_t c )
		{
			Serial.print( "WRITE " );
			Serial.println( c );

			uint8_t p = press( c );
			release( c );
			return ( p );
		}
		size_t	print( String str )
		{
			Serial.print( "TEXT " );
			Serial.println( str );
			return ( 1 );
		}
		size_t	println( String str )
		{
			Serial.print( "STRING " );
			Serial.println( str );
			return ( 1 );
		}
		size_t	release( uint8_t k )
		{
			Serial.print( "RELEASE " );
			Serial.println( k );
			return ( 1 );
		}
		void	releaseAll( void )
		{
			Serial.println( "RELEASEALL" );
			return ;
		}
};
# endif

String		keys					= "LEFT_CTRL=128" \
"LEFT_SHIFT=129" \
"LEFT_ALT=130" \
"LEFT_GUI=131" \
"RIGHT_CTRL=132" \
"RIGHT_SHIFT=133" \
"RIGHT_ALT=134" \
"RIGHT_GUI=135" \
"UP_ARROW=218" \
"DOWN_ARROW=217" \
"LEFT_ARROW=216" \
"RIGHT_ARROW=215" \
"BACKSPACE=178" \
"TAB=179" \
"RETURN=176" \
"ESC=177" \
"INSERT=209" \
"DELETE=212" \
"PAGE_UP=211" \
"PAGE_DOWN=214" \
"HOME=210" \
"END=213" \
"CAPS_LOCK=193" \
"F1=194" \
"F2=195" \
"F3=196" \
"F4=197" \
"F5=198" \
"F6=199" \
"F7=200" \
"F8=201" \
"F9=202" \
"F10=203" \
"F11=204" \
"F12=205" \
"";

/* meaningful names for constants used in this sketch
const byte	SC_SPACE				= 0x2c;
const byte	SC_ENTER				= 0x28;
const byte	SC_BCKSPC				= 0x2a;
const byte	SC_TAB					= 0x2b;
const byte	SC_ESCAPE				= 0x29;
const byte	SC_ESC					= 0x29;
const byte	SC_RIGHT_ARROW			= 0xef;
const byte	SC_LEFT_ARROW			= 0x50;
const byte	SC_DOWN_ARROW			= 0x51;
const byte	SC_UP_ARROW				= 0x52;
const byte	SC_F1					= 0x3a;
const byte	SC_F2					= 0x3b;
const byte	SC_F3					= 0x3c;
const byte	SC_F4					= 0x3d;
const byte	SC_F5					= 0x3e;
const byte	SC_F6					= 0x3f;
const byte	SC_F7					= 0x40;
const byte	SC_F8					= 0x41;
const byte	SC_F9					= 0x42;
const byte	SC_F10					= 0x43;
const byte	SC_F11					= 0x44;
const byte	SC_F12					= 0x45;

const byte	MOD_SHIFT				= 0xf1;
const byte	MOD_ALT					= 0xf2;
const byte	MOD_CTRL				= 0xf0;
const byte	SC_PERIOD				= 0x37;
const byte	SC_COMMA				= 0x36;
const byte	SC_SEMI_AND_COLON		= 0x33;
const byte	SC_SLASH_AND_QUESTION	= 0x38;

// not working
const byte	SC_VOL_UP				= 0x80;
const byte	SC_VOL_DOWN				= 0x81;
const byte	SC_MUTE					= 0x7f;
*/

/* Passer en clavier AZERTY (ne fonctionne pas)
const uint8_t _asciimap[128] =
{
	0x00, // NUL
	0x00, // SOH
	0x00, // STX
	0x00, // ETX
	0x00, // EOT
	0x00, // ENQ
	0x00, // ACK
	0x00, // BEL
	0x2a, // BS	Backspace
	0x2b, // TAB	Tab
	0x28, // LF	Enter
	0x00, // VT
	0x00, // FF
	0x00, // CR
	0x00, // SO
	0x00, // SI
	0x00, // DEL
	0x00, // DC1
	0x00, // DC2
	0x00, // DC3
	0x00, // DC4
	0x00, // NAK
	0x00, // SYN
	0x00, // ETB
	0x00, // CAN
	0x00, // EM
	0x00, // SUB
	0x00, // ESC
	0x00, // FS
	0x00, // GS
	0x00, // RS
	0x00, // US
	0x2c, //  ' '
	0x38, // !
	0x20, // "
	0x20, // # :TODO
	0x30, // $
	0x34|MOD_SHIFT, // %
	0x1E, // &
	0x21, // '
	0x22, // (
	0x2d, // )
	0x31, // * : done
	0x2b|MOD_SHIFT, // +
	0x10, // ,
	0x23, // -
	0x36|MOD_SHIFT, // .
	0x37|MOD_SHIFT, // /
	0x27|MOD_SHIFT, // 0
	0x1e|MOD_SHIFT, // 1
	0x1f|MOD_SHIFT, // 2
	0x20|MOD_SHIFT, // 3
	0x21|MOD_SHIFT, // 4
	0x22|MOD_SHIFT, // 5
	0x23|MOD_SHIFT, // 6
	0x24|MOD_SHIFT, // 7
	0x25|MOD_SHIFT, // 8
	0x26|MOD_SHIFT, // 9
	0x37, // :
	0x36, // ;
	0x64, // < Done
	0x2e, // =
	0x64|MOD_SHIFT, // > Done
	0x10|MOD_SHIFT, // ? 0x38 -> 0x10 OK
	0x1f, // @ TODO
	0x14|MOD_SHIFT, // A
	0x05|MOD_SHIFT, // B
	0x06|MOD_SHIFT, // C
	0x07|MOD_SHIFT, // D
	0x08|MOD_SHIFT, // E
	0x09|MOD_SHIFT, // F
	0x0a|MOD_SHIFT, // G
	0x0b|MOD_SHIFT, // H
	0x0c|MOD_SHIFT, // I
	0x0d|MOD_SHIFT, // J
	0x0e|MOD_SHIFT, // K
	0x0f|MOD_SHIFT, // L
	0x33|MOD_SHIFT, // M
	0x11|MOD_SHIFT, // N
	0x12|MOD_SHIFT, // O
	0x13|MOD_SHIFT, // P
	0x04|MOD_SHIFT, // Q
	0x15|MOD_SHIFT, // R
	0x16|MOD_SHIFT, // S
	0x17|MOD_SHIFT, // T
	0x18|MOD_SHIFT, // U
	0x19|MOD_SHIFT, // V
	0x1d|MOD_SHIFT, // W
	0x1b|MOD_SHIFT, // X
	0x1c|MOD_SHIFT, // Y
	0x1a|MOD_SHIFT, // Z
	0x0c, // [ TODO 2F
	0x31, // bslash
	0x0d, // ] TODO 30
	0x2F, // ^
	0x25, // _
	0x35, // ` TODO
	0x14, // a
	0x05, // b
	0x06, // c
	0x07, // d
	0x08, // e
	0x09, // f
	0x0a, // g
	0x0b, // h
	0x0c, // i
	0x0d, // j
	0x0e, // k
	0x0f, // l
	0x33, // m
	0x11, // n
	0x12, // o
	0x13, // p
	0x04, // q
	0x15, // r
	0x16, // s
	0x17, // t
	0x18, // u
	0x19, // v
	0x1d, // w
	0x1b, // x
	0x1c, // y
	0x1a, // z
	0x2f|MOD_SHIFT, //
	0x31|MOD_SHIFT, // | TODO
	0x30|MOD_SHIFT, // } TODO
	0x35|MOD_SHIFT, // ~ TODO
	0 // DEL
};
*/

#endif
