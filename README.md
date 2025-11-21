# ⏳ FocusWork 2.0  - Global Solution 2025

> **Solução de Edge Computing para Saúde e Produtividade no Futuro do Trabalho.**

## 👥 Autor
* **Nicolas Forcione de Oliveira e Souza** - RM-56998

## 📝 Descrição do Projeto
O **FocusWork 2.0** é um dispositivo inteligente de mesa focado no bem-estar do trabalhador em regime de Home Office ou Híbrido. 

Diferente de timers comuns, ele atua como um "Guardião do Ambiente": utiliza sensores para monitorar a qualidade do espaço de trabalho (temperatura e iluminação) em tempo real. Se as condições se tornarem insalubres (muito calor, frio ou má iluminação), o dispositivo **interrompe automaticamente** a sessão de trabalho, forçando uma pausa até que o ambiente seja corrigido.

## 🎯 O Desafio: O Futuro do Trabalho
Com o aumento do trabalho remoto, muitos profissionais negligenciam a ergonomia ambiental, resultando em:
* Fadiga ocular (iluminação inadequada).
* Queda de produtividade cognitiva (temperatura incorreta).
* Burnout (falta de pausas).

Nossa solução aborda esses problemas usando **Edge Computing** (processamento local no Arduino), garantindo privacidade total dos dados do usuário, sem depender de nuvem.

## 🛠️ Tecnologias e Hardware Utilizados
O projeto foi desenvolvido em C++ utilizando a plataforma Arduino.

| Componente | Função |
| --- | --- |
| **Arduino Uno** | Processamento central (Edge Computing). |
| **Sensor DHT22** | Monitoramento de Temperatura e Umidade. |
| **Sensor LDR** | Monitoramento de Luminosidade (Lux). |
| **LCD 16x2 (I2C)** | Interface visual e feedback ao usuário. |
| **Teclado Matricial 4x4** | Navegação de menus e configuração de tempo. |
| **Buzzer** | Alertas sonoros de fim de ciclo e perigo. |
| **LEDs (Verde/Vermelho)** | Feedback visual de status do ambiente. |

## ⚙️ Funcionalidades do Sistema

1.  **Timer Pomodoro Configurável:** * Presets rápidos (10, 25, 50 min) ou tempo personalizado via teclado numérico.
2.  **Monitoramento Ambiental Ativo:**
    * **Limites de Segurança:** Temperatura (10°C - 30°C) e Luz (200 - 1000 lux).
3.  **Interrupção de Segurança:**
    * Se o ambiente ficar ruim, o timer **PAUSA** automaticamente e o LED Vermelho acende.
    * O sistema só permite continuar quando o ambiente é corrigido (Feedback com LED Verde).
4.  **Interface Intuitiva:**
    * Sistema de menus completo e tecla de Ajuda ('D').

## 🚀 Como Rodar a Simulação (Wokwi)

1. Acesse o site [Wokwi.com]([https://wokwi.com](https://wokwi.com/projects/448141936821896193).
2. Crie um novo projeto com Arduino Uno.
3. Copie o código do arquivo `src/main.ino` deste repositório.
4. Monte o circuito conforme o diagrama (ou copie o `diagram.json`).
5. Instale as bibliotecas necessárias no Library Manager:
    * `LiquidCrystal I2C`
    * `Keypad`
    * `DHT sensor library`

## 📼 Assitea o video no Youtube
Acesse o video: https://youtu.be/hAQH3jOOA5w

## 🌍 Conexão com ODS (ONU)
Este projeto está alinhado com:
* **ODS 3 (Saúde e Bem-Estar):** Previne doenças ocupacionais e estresse.
* **ODS 8 (Trabalho Decente):** Promove ambientes de trabalho seguros e produtivos.

---
*Obrigado pela sua atenção! 😁* 
