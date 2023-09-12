## About the project ##

The project provides a simple HTTP server for handling requests to generate text with any of the available GGML LLaMA models.

## Running the server ##

To run the server you can follow these steps:

** 1) Install the ctransformers library

You can do it by following the installation guide at https://github.com/marella/ctransformers

** 2) Download a LLaMA model

The server supposed to work with any LLaMA model with Alpaca style prompts.
Models that are confirmed to be working:
* https://huggingface.co/TheBloke/WizardLM-1.0-Uncensored-Llama2-13B-GGML
* https://huggingface.co/TheBloke/llama2_7b_chat_uncensored-GGML

** 3) Configure the server

You can configure the server by editing the `config.json` file

```json
{
    "threads"       : 1,
    "gpu"           : 0,
    "model-path"    : "path/to/your/llama/model",
    "host"          : "127.0.0.1",
    "port"          : 5000
}
```

* `threads` - number of CPU threads
* `gpu` - number of GPU layers
* `model-path` - path to your model
* `host` - server host
* `port` - server port

** 4) Run the server!

Start the server by running `server.py`. Now, you can send request to it from your client.

A simple client can be started by running `client.py`. In the future, more advanced GUI client would be added too.