  // Header files
  // -----------
  // SPI ethernet library
  // controller : P18F4620  , Ethernet card : ENC28J60
 // Blinking a LED after reciving a message (B1# or B2#) from the client

  #include "__EthEnc28j60.h"

  //Declaration

  // Led

  sbit LED at LATD5_bit;
  sbit LED_Direction at TRISD5_bit;

  // Ethernet Module

  sbit SPI_Ethernet_Rst at LATC0_bit;
  sbit SPI_Ethernet_CS at LATC1_bit;
  sbit SPI_Ethernet_Rst_Direction at TRISC0_bit;
  sbit SPI_Ethernet_CS_Direction at TRISC1_bit;

  //Define FULL DUPLEX
  #define FULLDUPLEX     1
  unsigned char MAC_Address[6] ={0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f};

  // IP address depends on the router
  unsigned char IP_Address[4]={192, 168, 1, 10};

  // Prototypes

  void Initialize(void);
  void Ethernet_doPacket(void);


  void Initialize (void)
  {

   LED=0;

   // Ethernet
   SPI_Ethernet_Rst_Direction =0;
   SPI_Ethernet_CS_Direction=0;

   // Libraries

   SPI1_init();
   SPI_Ethernet_Init(MAC_Address,IP_Address ,FULLDUPLEX);


  }


     void Ethernet_doPacket(void)
     {
           volatile unsigned char received_packet_check =0;
           
           // check the return value
           
           received_packet_check =SPI_Ethernet_doPacket();
           
           if(received_packet_check==1)
           {
              SPI_Ethernet_Rst=0;     // Low
              Delay_ms(100);
              SPI_Ethernet_Rst=1;     //high
              
              SPI1_Init();
              SPI_Ethernet_Init(MAC_Address,IP_Address ,FULLDUPLEX);
           }
     }

  unsigned int    SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags * flags)
  {
  // No reply to the TCP requests
  return (0);

  }
  
   unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags * flags)
  {
     unsigned char buffer[3] ={0};
     unsigned char i=0;
     
     if (localPort !=60000) {
     return 0;
     }
     
     for (i=0;i<3;i++)
     {
     buffer[i]=SPI_Ethernet_getByte();
     }
     
      if (buffer[0] =='B' && buffer[2] =='#')
      {
              switch (buffer[1])
              {
                     case '1':
                          LED=1;
                          break;
                      case '2':
                           LED=0;
                           break;
                      default :
                           ;    // Do Nthg
              
              }
      
      
      }
  
  return (0);
  }
  
  
  
  

  void main(void) {
       Initialize();
       
       while(1){
            Ethernet_doPacket();
       }
       
  }