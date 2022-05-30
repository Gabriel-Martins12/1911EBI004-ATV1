#include <stdint.h>

#define SRAM_START 0x20000000U      /* Inicio da SRAM CORTEX-M */
#define SRAM_SIZE (128U * 1024U)    /* Tam. SRAM STM32F411 128K */
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))      /* Final da SRAM STM32F411 */

#define STACK_START SRAM_END        /* Inicio da Stack */
int main(void);

void reset_handler (void);
//Vamos, inicialmente, remover a implementação da função nmi_handler() e modificar seu o protótipo de forma que se torne um alias para a função default_handler().
//weak vem para solucionar o problema de precisar reescrever toda vez a startup
void nmi_handler (void) __attribute__ ((weak, alias("default_handler")));
void hardfault_handler (void) __attribute__ ((weak, alias("default_handler")));
void memmanage_handler (void) __attribute__ ((weak, alias("default_handler")));
void busfault_handler (void) __attribute__ ((weak, alias("default_handler")));
void usagefault_handler(void) __attribute__ ((weak, alias("default_handler")));
void svc_handler (void) __attribute__ ((weak, alias("default_handler")));
void debugmon_handler (void) __attribute__ ((weak, alias("default_handler")));
void pendsv_handler (void) __attribute__ ((weak, alias("default_handler")));
void systick_handler (void) __attribute__ ((weak, alias("default_handler")));

/*Variáveis que serão exportadas pelo linker script*/
extern uint32_t _sdata; //Inicio da secao .data
extern uint32_t _edata; //Fim da secao .data
extern uint32_t _la_data; //Endereço da carga na RAM da secao .data

extern uint32_t _sbss; //Inicio da secao .bss
extern uint32_t _ebss; //Fim de .bss

/*Para copiar os dados da seção .data para a memória SRAM é ,
basta iterarmos por toda a seção na memória FLASH e copiar os dados para o
endereço de destino na memória SRAM. Da mesma forma, para escrever zero
na seçao .bss iremos iterar por toda a seção na SRAM escrevendo zero em
cada posição. Ao final deste processo basta chamar a função main(). Lembre
que a função main() será implementada em outro arquivo logo, é necessário
definirmos seu protótipo. - TEXTO DO ROTEIRO 02*/

uint32_t vectors[] __attribute__((section(".isr_vectors"))) =
{
    STACK_START,                     /* 0x0000 0000 */
    (uint32_t)reset_handler,         /* 0x0000 0004 */
    (uint32_t)nmi_handler,           /* 0x0000 0008 */
    (uint32_t)hardfault_handler,     /* 0x0000 000c */
    (uint32_t)memmanage_handler,     /* 0x0000 0010 */
    (uint32_t)busfault_handler,      /* 0x0000 0014 */
    (uint32_t)usagefault_handler,    /* 0x0000 0018 */
    0,                              /* 0x0000 001c */
    0,                              /* 0x0000 0020 */
    0,                              /* 0x0000 0024 */
    0,                              /* 0x0000 0028 */
    (uint32_t)svc_handler,           /* 0x0000 002c */
    (uint32_t)debugmon_handler,      /* 0x0000 0030 */
    0,                              /* 0x0000 0034 */
    (uint32_t)pendsv_handler,       /* 0x0000 0038 */
    (uint32_t)systick_handler,       /* 0x0000 003c */
};

//Agora, após finalizar a Tabela de Vetores de Interrupção - código de inicialização na função abaixo
void reset_handler(void)
//Copia dados da seção .data para SRAM
//Preenche com 0 a seção .bss
//Chama a main()
{
    uint32_t i;
    /* Copia a secao .data para a RAM */
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint8_t *pDst = (uint8_t*)&_sdata; /* SRAM */
    uint8_t *pSrc = (uint8_t*)&_etext; /* FLASH */
    for(i = 0; i < size; i++)
    {
    *pDst++ = *pSrc++;
    }
    /* Preenche a secao .bss com zero */
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t*)&_sbss;
    for(i = 0 ; i < size; i++)
    {
        *pDst++ = 0;
    }
    /* Chama a funcao main() */
    main();
}
//Excluindo a nmi_handler
/*void nmi_handler(void)
{
    while(1){};
}*/

void default_handler(void)
{
    while(1){};
}