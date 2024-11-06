+++
title = 'Projeto'
type = 'page'
date = 2024-09-22T01:02:39-03:00
draft = false
+++

# Overview
O projeto consiste em uma balança de bioimpedância. Ela será capaz de medir a gordura corporal, massa magra e massa de água do corpo. 


## Proposta
A balança fará uso de um ESP32 e o CI AD5933.
Além disso, haverá um aplicativo de celular com o qual a balança irá se conectar.
O aplicativo será escrito em React Native, e incorporará um banco de dados SQLite.

### Requisitos
#### Requisitos Funcionais
- **RF1**: o usuário deve ser capaz de ligar e desligar o sistema
- **RF2**: o sistema deve ser capaz de pesar o usuário
- **RF3**: o usuário deve ser capaz de visualizar seu peso na balança
- **RF4**: o sistema deve ser capaz de medir a impedância corporal do usuário
- **RF5**: o sistema deve ser capaz de fazer uma análise de impedância bioelétrica (BIA), calculando dados de composição corporal como massa gorda, massa muscular e massa de água corporal
- **RF6**: o usuário deve ser capaz de conectar o celular com o sistema
- **RF7**: o sistema deve ser capaz de fornecer os dados da BIA por meio de um aplicativo de celular
- **RF8**: o usuário deve ser capaz de visualizar os dados de BIAs passadas no aplicativo de celular
- **RF9**: o usuário deve poder inserir seus dados pessoais, tais como idade e altura, por meio do aplicativo
- **RF10**: o usuário deve ser capaz de descartar ou salvar a medição realizada
- **RF11**: o usuário deve ser capaz de acompanhar os avanços nas etapas de medição
- **RF12**: o sistema deve permitir a adição de novos usuários

#### Requisitos Não Funcionais
- **RF1**: o microcontrolador deve se conectar ao celular do usuário via BLE (Bluetooth Low Energy)
- **RF2**: o aplicativo deve ser compatível com sistemas Android
- **RF3**: o sistema deve mostrar o peso do usuário através de um display 7 segmentos
- **RF4**: o armazenamento do s dados do aplicativo será feito via banco de dados relacional SQLite
- **RF5**: o sistema deve exibir uma página para cadastrar novos usuários
- **RF6**: a alimentação dos módulos, CIs e microcontroladores será realizada utilizando pilha
- **RF7**: a interface do aplicativo deve ser intuitiva e fácil de usar
- **RF8**: o circuito auxiliar deve conter um conversor de impedância AD5933
- **RF9**: o aplicativo deve exibir uma página com os resultados ao finalizar a medição




A proposta completa do projeto pode ser encontrada [aqui](/proposta.pdf).


# Cronograma
O cronograma pode ser encontrado [aqui](https://docs.google.com/spreadsheets/d/1uDyi9QAd5F_Qd_09Ph9dPdHUsxXU9gyrMMj6czyDvkA/edit?gid=0#gid=0).

