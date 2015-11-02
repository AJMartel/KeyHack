#include	<SPI.h>
#include	<SD.h>

#include	"KeyHack.h"

#define		PIN_SD		4
#define		PIN_ETH		10
#define		PIN_LED		9

#define		PIN_OS0		7
#define		PIN_OS1		6
#define		PIN_OS2		5

#define		PIN_RL0		A0
#define		PIN_RL1		A1

File	list;

int		pf		= -1;
int		wl		= -1;

int		t_pf;
int		t_wl;

int		f_pf	= -1;
int		f_os	= 0; // à désactiver

String	stmp	= "";

#if !defined( __AVR_ATmega32U4__ )
SimulKeyboard	Keyboard;
#endif

void	setup()
{
	char			*file		= NULL;
	SdFile			root;
	Sd2Card			card;
	SdVolume		volume;

	Keyboard.begin();

	pinMode( PIN_LED, OUTPUT );
	pinMode( PIN_OS0, INPUT );
	pinMode( PIN_OS1, INPUT );
	pinMode( PIN_OS2, INPUT );

	digitalWrite( PIN_RL0, HIGH );
	digitalWrite( PIN_RL1, HIGH );
	pinMode( PIN_RL0, OUTPUT );
	pinMode( PIN_RL1, OUTPUT );

	pinMode( PIN_ETH, OUTPUT );
	digitalWrite( PIN_ETH, HIGH );

	bool sd = false;
	if ( !card.init( SPI_HALF_SPEED, PIN_SD ) && ( sd = true ) )
		Serial.println( F( "# SD Not readable" ) );
	else if ( !volume.init( card ) && ( sd = true ) )
		Serial.println( F( "# SD Not correct" ) );
	if ( sd )
		end();

	SD.begin( PIN_SD );
	root.openRoot( volume );
	if ( f_os == -1 && ( f_pf = osSelected() ) == -1 )
	{
		Serial.begin( 9600 );
		while ( !Serial )
			fadeLed();

		Serial.println( F( "# KeyHack SD files" ) );
		listFolder( root );

		while ( 42 )
		{
			Serial.print( F( "?> " ) );
			while ( !Serial.available() )
				fadeLed();

			if ( ( file = fileSelected( true ) ) )
			{
				if ( SD.exists( file ) )
					break ;
				else
				{
					free( file );
					file = NULL;
				}
			}

			Serial.println( F( "# File Not found" ) );
		}

		list = SD.open( file, FILE_READ );
		free( file );
	}
	else
	{
		file = fileSelected();
		list = SD.open( file ? file : "script.kh", FILE_READ );
	}

	if ( !list )
	{
		Serial.println( F( "# File Error" ) );
		end();
	}

	f_pf = osSelected();
	if ( f_pf == 0 || f_os == 0 ) // à vérifier
	{
		Keyboard.press( 129 ); // shift gauche
		Keyboard.press( 0x36 + 136 ); //  ',<' config clavier OS X
		Keyboard.releaseAll();
	}

	return ;
}

void	loop()
{
	int				pos;
	bool			check;
	String			current;
	String			command;
	String			params;
	unsigned int	line;

	Serial.println( F( "# Start File" ) );
	while ( list && list.available() )
	{
		current = readLine();
		line = list.position();

		current.trim();
		if ( !current.length() )
			continue ;

		check = true;
		command = readCommand( current );
		params = readParams( current );

		if ( command == F( "END" ) )
		{
			if ( params == F( "PLATEFORM" ) && pf >= 0 )
			{
				pf = -1;
				check = t_pf == f_os || t_pf == f_pf;
			}
			else if ( params == F( "WHILE" ) && wl >= 0 )
			{
				if ( --t_wl <= 0 )
					wl = -1;
				else
					list.seek( wl );
			}
			else
				check = false;
		}
		else if ( pf == -1 || ( pf >= 0 && ( ( f_os < 0 && t_pf == f_pf ) || t_pf == f_os ) ) )
		{
			if ( command == F( "REM" ) )
			{
				Serial.print( params );
				Serial.println();
			}
			else if ( command == F( "LED" ) )
			{
				if ( params == "1" )
					digitalWrite( PIN_LED, HIGH );
				else if ( params == "0" )
					digitalWrite( PIN_LED, LOW );
			}
			else if ( command == F( "DELAY" ) )
				delay( atoi( params.c_str() ) );
			else if ( command == F( "TEXT" ) )
				Keyboard.print( params );
			else if ( command == F( "STRING" ) )
				Keyboard.println( params );
			else if ( command == F( "PRESS" ) || command == F( "WRITE" ) )
			{
				keysSend( params );
				if ( command == F( "WRITE" ) )
					Keyboard.releaseAll();
			}
			else if ( command == F( "RELEASE" ) )
			{
				if ( params == F( "ALL" ) )
					Keyboard.releaseAll();
				else
					keysSend( params, true );
			}
			else if ( command == F( "PLATEFORM" ) )
			{
				int on = 0;
				if ( pf == -1 )
				{
					if ( params == F( "OSX" ) && ( on = 1 ) )
						t_pf = 0;
					else if ( params == F( "LINUX" ) && ( on = 1 ) )
						t_pf = 1;
					else if ( params == F( "WINDOWS" ) && ( on = 1 ) )
						t_pf = 2;
					else if ( params == F( "ANDROID" ) && ( on = 1 ) )
						t_pf = 3;
					else if ( params == F( "UBUNTU" ) && ( on = 1 ) )
						t_pf = 4;

					pf = on ? line : pf;
				}
				check = pf != -1 && ( t_pf == f_os || t_pf == f_pf );
			}
			else if ( command == F( "WHILE" ) )
			{
				t_wl = atoi( params.c_str() );
				if ( wl == -1 && t_wl > 0 )
					wl = line;
			}
			else
				check = false;
		}
		else
			check = false;

		if ( check )
		{
			Serial.print( "#> " + command );
			for ( int i = 3 - ( command.length() / 4 ); i > 0; --i )
				Serial.print( '\t' );
			Serial.println( params );
		}
	}

	Serial.println( F( "# End File" ) );
	end();

	return ;
}

void	end( void )
{
	list.close();
	Keyboard.end();
	Serial.end();

	// Pour switch la DATA du port USB vers HDD
	digitalWrite( PIN_RL0, LOW );
	digitalWrite( PIN_RL1, LOW );

	digitalWrite( PIN_LED, HIGH );
	while ( 42 )
		;
	return ;
}

void	fadeLed( void )
{
	for ( int i = 0; i < 480; ++i )
	{
		analogWrite( PIN_LED, ( ( i >= 240 ) ? 480 - i : i ) );
		delay( 8 );
	}

	return ;
}

int		osSelected( void )
{
	int				os		= 0;

	os += digitalRead( PIN_OS0 ) ? 0 : 1;
	os += digitalRead( PIN_OS1 ) ? 0 : 10;
	os += digitalRead( PIN_OS2 ) ? 0 : 100;

	if ( os == 1 )			// OSX
		return ( 0 );
	else if ( os == 10 )	// LINUX
		return ( 1 );
	else if ( os == 11 )	// WINDOWS
		return ( 2 );
	else if ( os == 100 )	// ANDROID
		return ( 3 );
	else if ( os == 101 )	// UBUNTU
		return ( 4 );

	return ( -1 );
}

char	*fileSelected( void )
{
	return ( fileSelected( false ) );
}

char	*fileSelected( bool wait )
{
	int				i			= 0;
	int				pos;
	bool			reset		= false;
	char			*file		= ( char * ) malloc( sizeof( char ) * 12 );

	memset( file, 0, sizeof( char ) * 12 );
	if ( wait )
	{
		while ( Serial.available() )
		{
			file[ i ] = Serial.read();
			file[ i ] += ( file[ i ] >= 'A' && file[ i ] <= 'Z' ) ? ( 'a' - 'A' ) : 0;

			if ( file[ i ] <= ' ' )
			{
				Serial.flush();
				file[ i ] = '\0';
			}
			++i;
		}
	}

	if ( !file[ 0 ] || !SD.exists( file ) )
	{
		reset = true;

		if ( SD.exists( "config.ini" ) )
		{
			list = SD.open( "config.ini", FILE_READ );
			while ( reset && list && list.available() )
			{
				stmp = list.readString();
				stmp.trim();

				if ( ( pos = stmp.indexOf( "default=" ) ) >= 0 )
				{
					stmp = stmp.substring( stmp.indexOf( '=' ) + 1 );

					memset( file, 0, sizeof( char ) * 12 );
					strcpy( file, stmp.c_str() );

					if ( SD.exists( file ) )
						reset = false;
					break ;
				}
			}
			list.close();
		}
	}

	if ( reset )
	{
		free( file );
		file = NULL;
	}

	return ( file );
}

void	listFolder( SdFile dir )
{
	dir_t			tmp;
	char			ext[ 4 ]	= {};
	char			file[ 9 ]	= {};

	dir.rewind();
	while ( dir.readDir( tmp ) > 0 )
	{
		if ( tmp.name[ 0 ] == DIR_NAME_FREE )
			break;
		else if ( tmp.name[ 0 ] == DIR_NAME_DELETED || tmp.name[ 0 ] == '.' )
			continue;
		else if ( !DIR_IS_FILE_OR_SUBDIR( &tmp ) )
			continue;

		for ( int i = 0; i < 11; ++i )
		{
			tmp.name[ i ] += ( tmp.name[ i ] > 65 && tmp.name[ i ] < 90 ) ? 32 : 0;

			if ( i >= 8 )
				ext[ i - 8 ] = ( tmp.name[ i ] == ' ' ) ? '\0' : tmp.name[ i ];
			else
				file[ i ] = ( tmp.name[ i ] == ' ' ) ? '\0' : tmp.name[ i ];
		}

		if ( ext[ 0 ] == 'k' && ext[ 1 ] == 'h' && !ext[ 2 ] )
		{
			Serial.print( " - " );
			Serial.print( file );
			if ( ext[ 0 ] )
			{
				Serial.write( '.' );
				Serial.print( ext );
			}
			Serial.println();
		}
	}

	return ;
}

String	readLine( void )
{
	char			c;
	String			str;

	while ( list && list.available() )
	{
		c = list.read();
		if ( c == '\n' || c == '\0' )
			return ( str );

		str += c;
	}

	return ( str );
}

String	readCommand( String str )
{
	int				pos		= str.indexOf( ' ' );
	int				pos2	= str.indexOf( '\t' );

	pos = ( pos == -1 ) ? pos2 : pos;
	pos = ( pos2 == -1 || pos2 > pos ) ? pos : pos2;
	if ( pos <= 0 && pos2 <= 0 )
	{
		return ( str );
	}

	return ( str.substring( 0, pos ) );
}

String	readParams( String str )
{
	int				pos		= str.indexOf( ' ' );
	int				pos2	= str.indexOf( '\t' );

	pos = ( pos == -1 ) ? pos2 : pos;
	pos = ( pos2 == -1 || pos2 > pos ) ? pos : pos2;
	if ( pos <= 0 && pos2 <= 0 )
		return ( "" );

	const char *tmp = str.c_str();
	while ( tmp[ pos ] == ' ' || tmp[ pos ] == '\t' )
		++pos;

	return ( str.substring( pos ) );
}

void	keysSend( String str )
{
	keysSend( str, false );
	return ;
}

void	keysSend( String str, bool release )
{
	int key;
	int pos;

	stmp = "";
	while ( ( pos = str.indexOf( ' ' ) ) > 0 )
	{
		stmp = readCommand( str );
		str = readParams( str );
		pos = keys.indexOf( stmp.c_str() );

		if ( pos >= 0 )
		{
			stmp = keys.substring( keys.indexOf( '=', pos ), keys.indexOf( '\n', pos ) );
			if ( !stmp || !( key = atoi( stmp.c_str() ) ) )
				continue ;

			if ( release )
				Keyboard.release( key );
			else
				Keyboard.press( key );
		}
		else if ( stmp.length() == 1 )
		{
			if ( release )
				Keyboard.release( stmp.c_str()[ 0 ] );
			else
				Keyboard.press( stmp.c_str()[ 0 ] );
		}
	}

	return ;
}
