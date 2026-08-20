
#CAN端口先启用
ip link set can0 down 
ip link set can0 type can bitrate 1000000 
ip link set can0 up

ip link set can1 down 
ip link set can1 type can bitrate 1000000 
ip link set can1 up

ip link set can2 down 
ip link set can2 type can bitrate 1000000 
ip link set can2 up

ip link set can3 down 
ip link set can3 type can bitrate 1000000 
ip link set can3 up

ip link set can4 down 
ip link set can4 type can bitrate 1000000 
ip link set can4 up
    
ip link set can5 down 
ip link set can5 type can bitrate 1000000 
ip link set can5 up

ip link set can6 down 
ip link set can6 type can bitrate 1000000 
ip link set can6 up

ip link set can7 down 
ip link set can7 type can bitrate 1000000 
ip link set can7 up

ip link set can8 down 
ip link set can8 type can bitrate 1000000 
ip link set can8 up

ip link set can9 down 
ip link set can9 type can bitrate 1000000 
ip link set can9 up


#USART 
./build/usart/uart_node --name humiture --dev /dev/CH9344USB0 --baud 115200
./build/usart/uart_node --name barometer --dev /dev/CH9344USB1 --baud 115200
./build/usart/uart_node --name waterlevel --dev /dev/CH9344USB2 --baud 115200
./build/usart/uart_node --name singlebeam --dev /dev/CH9344USB3 --baud 115200
./build/usart/uart_node --name camera --dev /dev/CH9344USB4 --baud 115200
./build/usart/uart_node --name navigation --dev /dev/CH9344USB5 --baud 115200
./build/usart/uart_node --name aux --dev /dev/CH9344USB6 --baud 115200
./build/usart/uart_node --name svp --dev /dev/CH9344USB7 --baud 115200
./build/usart/uart_node --name sidehigh --dev /dev/CH9344USB8 --baud 115200
./build/usart/uart_node --name side1 --dev /dev/CH9344USB9 --baud 115200
./build/usart/uart_node --name side2 --dev /dev/CH9344USB10 --baud 115200
./build/usart/uart_node --name svp70 --dev /dev/CH9344USB11 --baud 115200


#CAN
./build/can/can_node --name bow_battery --ifname can0
./build/can/can_node --name battery --ifname can1
./build/can/can_node --name fore_pump --ifname can2
./build/can/can_node --name fore_valve --ifname can3

#PWM
./build/pwm/pwm_node --chip 0 --period 1000000000 --duty 500000000 --polarity normal --enable 1
./build/pwm/pwm_node --chip 1 --period 1000000 --duty 500000 --polarity normal --enable 1
./build/pwm/pwm_node --chip 2 --period 1000000 --duty 500000 --polarity normal --enable 1
./build/pwm/pwm_node --chip 3 --period 1000000 --duty 500000 --polarity normal --enable 1
./build/pwm/pwm_node --chip 4 --period 1000000 --duty 500000 --polarity normal --enable 1

#漏水入水检测
./build/gpio/water_node  --name water