# 🏍️ IOT_SPRINT_03

Protótipo funcional de IoT desenvolvido para a 3ª Sprint da disciplina **Disruptive Architectures: IoT, IoB & Generative IA**.  
O sistema integra **simuladores IoT de motos**, **backend FastAPI** e **dashboard Streamlit**, com comunicação em tempo real via **MQTT** e persistência em banco de dados.

---

## 📖 Descrição

O projeto simula um **ecossistema de mobilidade conectado**, onde motos são monitoradas por diferentes dispositivos IoT e possuem **status operacionais dinâmicos**.  

- **Simuladores de dispositivos IoT**:  
  - **Dock Sensor** → informa status da vaga (livre/ocupada) e qual moto está estacionada.  
  - **Portal RFID** → controla entradas e saídas de motos no pátio.  
  - **Simulador de manutenção** → altera o status de uma moto para manutenção ou pronta.  

- **Status das motos**:  
  - `Moto no pátio`  
  - `Moto fora do pátio`  
  - `Moto em manutenção`  
  - `Moto pronta para uso`  

- **Broker MQTT (Mosquitto/HiveMQ)** → intermedia mensagens entre dispositivos e backend.  
- **Backend (FastAPI)** → processa eventos e salva no banco (SQLite/Postgres).  
- **Dashboard (Streamlit)** → exibe em tempo real os status das motos e os eventos.  
- **Node-RED** → opcional, usado para assinar tópicos MQTT e montar fluxos de apoio.  

---

## 🎯 Funcionalidades

- 📡 Comunicação em tempo real entre simuladores IoT e backend via MQTT.  
- 🛢️ Persistência de eventos: entradas/saídas (RFID), ocupação de docks e manutenção.  
- 📊 Dashboard atualizado em tempo real mostrando **status de cada moto**.  
- 🧪 Casos de teste funcionais:  
  - **Moto desaparecida** → ausência de atualização esperada.  
  - **Moto em vaga errada** → dock reporta moto diferente da última entrada.  
  - **Moto em manutenção** → alteração de status bloqueia uso da moto.  

---

## 🖼️ Ilustrações

- 📡 **Diagrama dos dispositivos IoT (motos)** → `docs/diagrama_sensor.png`  
- 🌐 **Fluxo Node-RED** → `docs/fluxo_node.png`  
- 🖥️ **Arquitetura IoT** → `docs/architecture.md`  

---

## ⚙️ Requisitos Técnicos

- **Software**  
  - Python 3.10+ (FastAPI, asyncio-mqtt, Streamlit, SQLAlchemy, aiosqlite)  
  - Docker + Docker Compose  
  - Broker MQTT (Mosquitto local ou HiveMQ público)  
  - Node-RED  

---

🛠️ Como executar o nosso projeto
✅ Simulação no Wokwi

Acesse Wokwi
.

Importe os arquivos do repositório:

sketch.ino e diagram.json → para o publicador (portal/dock/manutenção)

receptor.ino e diagram-receptor.json → para o receptor (status da moto)

Clique em Start Simulation.

Use os botões virtuais no diagrama:

Botão verde → alterna entrada/saída no portal.

Botão vermelho → alterna manutenção/pronta.

O LED amarelo → acende quando a moto está no pátio, apaga quando está fora.

Veja no Serial Monitor os eventos MQTT publicados e recebidos em tempo real.

🌐 Execução no Node-RED

Permite visualizar e processar os eventos das motos em dashboards interativos.

Instale o Node-RED:

npm install -g node-red
node-red


Acesse http://localhost:1880
 no navegador.

Importe o fluxo pronto (flows-mottu.json) que acompanha o repositório.

Configure os nós MQTT in com:

Broker: broker.hivemq.com

Porta: 1883

QoS: 0

Tópicos:

mottu/portal

mottu/dock

mottu/manutencao

Clique em Deploy.

Abra o dashboard em http://localhost:1880/ui
 para visualizar:

Evento Portal: entrada / saída

Status Dock: ocupada / livre

Status Manutenção: manutenção / pronta
