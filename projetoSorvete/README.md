# Simulador de Fábrica de Sorvete

Este projeto implementa um simulador de fábrica de sorvete usando WinAPI (C++).

## Requisitos

- Visual Studio 2019 ou superior
- Windows 10 ou superior
- C++17 ou superior

## Estrutura do Projeto

```
projetoSorvete/
│
├── bin/                        # Executáveis gerados
├── build/                      # Objetos e arquivos de build temporários
├── include/                    # Headers públicos
├── res/                        # Recursos (.rc, ícones, menus)
├── src/                        # Código-fonte
└── ProjetoSimSorvete.sln       # Solução do Visual Studio
```

## Build

1. Abra o arquivo `ProjetoSimSorvete.sln` no Visual Studio
2. Selecione a configuração "Release" e a plataforma "x64"
3. Clique em "Build Solution" (F7)

O executável será gerado em `bin/Release/ProjetoSimSorvete.exe`

## Uso

1. Execute o arquivo `ProjetoSimSorvete.exe`
2. Use os botões para controlar o processo:
   - Iniciar/Parar/Pausar/Retomar o processo
   - Abrir/Fechar válvulas dos tanques
3. Observe os níveis dos tanques, temperatura e pH na interface

## Funcionalidades

- Simulação de tanques (base, sabores, saída)
- Controle de válvulas
- Monitoramento de temperatura e pH
- Interface gráfica com animações
- Comunicação com hardware (simulada)

## Arquitetura

O projeto segue uma arquitetura em camadas:

- **UI**: Interface gráfica (MainWindow)
- **Controladores**: Lógica de negócio (ProcessController)
- **Serviços**: Funcionalidades auxiliares (TimerService, HardwareComms)
- **Modelo**: Estado da aplicação (StateModel)
- **Simulação**: Lógica de simulação (SimulationService)
- **Animação**: Renderização (AnimationEngine)
