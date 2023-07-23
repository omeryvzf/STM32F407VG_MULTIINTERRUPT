#include "stm32f4xx.h"
#include"stm32f4xx_exti.h"

void gpioConfig(){

	GPIO_InitTypeDef	GPIOInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	// led config
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIOInitStructure.GPIO_OType=GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2;
	GPIOInitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIOInitStructure);
	// button config
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIOInitStructure.GPIO_OType=GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_6;
	GPIOInitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC,&GPIOInitStructure);

}

void extiConfig(){
	EXTI_InitTypeDef	EXTIInitStructure;
	NVIC_InitTypeDef	NVICInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource6);

	//exti config
	EXTIInitStructure.EXTI_Line=EXTI_Line2 | EXTI_Line6;
	EXTIInitStructure.EXTI_LineCmd=ENABLE;
	EXTIInitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTIInitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //grup seçtik öncelik için
	// nvic ilk öncelikli buton config
	NVICInitStructure.NVIC_IRQChannel=EXTI2_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICInitStructure);
	// ikinci öncelikle buton confiig
	NVICInitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICInitStructure);
}

void delay(uint32_t time){
	while(time--);
}

void EXTI2_IRQHandler(){

	if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
		GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2);
		delay(36000000);


	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}
void EXTI9_5_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2);
		delay(36000000);


	}
	EXTI_ClearITPendingBit(EXTI_Line6);
}

int main(void){
	gpioConfig();
	extiConfig();

  while (1){
	  GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2);
	  delay(3600000);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2);
	  delay(3600000);



  }
}
