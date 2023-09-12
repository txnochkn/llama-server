import http.server
import socketserver
import json
from ctransformers import AutoModelForCausalLM

with(open("config.json") as file):
    config = json.loads(file.read())

MODEL_PATH=config['model-path']
PROMP_LENGTH=16384

llm=None

# Define the port number to listen on
port = 5000

# Create a custom HTTP request handler that handles POST requests
class MyHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers["Content-Length"])
        post_data = self.rfile.read(content_length).decode("utf-8")
        
        try:
            # Parse the JSON data
            json_data = json.loads(post_data)
            print("Received JSON data:")
            print(json.dumps(json_data))

            context = json_data['context']
            chat_history = str(json_data['history'])
            prompt = json_data['prompt']
            token_count = json_data['tokens']

            username = json_data['username']
            name = json_data['name']

            access_key = json_data['key']

            with(open("keys.json") as file):
                access_keys_json = file.read()

            access_keys = list(json.loads(access_keys_json))

            if(access_keys.count(access_key) == 0):
                self.send_response(401)
                self.send_header("Content-type", "text/plain")
                self.end_headers()
                self.wfile.write("Invalid access key!".encode())

                return

            context_len = len(context)
            prompt_len = len(prompt)

            history_max_len = PROMP_LENGTH-context_len-prompt_len

            chat_history = chat_history[-history_max_len:]
            firs_req = chat_history.find("### " + username)
            chat_history = chat_history[firs_req:]

            final_prompt = context + '\n' + chat_history + '### ' + username + ':\n' + prompt + '\n### ' + name + ':\n'
            
            response = llm(final_prompt, max_new_tokens=token_count, stop=["### " + username])

            response_data = {
                "response" : response,
                "history" : chat_history + '### ' + username + ':\n' + prompt + '\n### ' + name + ':\n' + response
            }

            response_str = json.dumps(response_data)
            # Send an HTTP response
            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.end_headers()
            self.wfile.write(response_str.encode())

        except json.JSONDecodeError as e:
            # If JSON parsing fails, send a 400 Bad Request response
            self.send_response(400)
            self.send_header("Content-type", "text/plain")
            self.end_headers()
            self.wfile.write(f"JSON parsing error: {str(e)}".encode("utf-8"))

# Set up the server to listen on the specified port with the custom handler
with socketserver.TCPServer((config['host'], config['port']), MyHandler) as httpd:
    llm = AutoModelForCausalLM.from_pretrained(MODEL_PATH, model_type="llama", threads = config['threads'], gpu_layers = config['gpu'])
    
    print(f"Serving at port {port}")
    # Start the server
    httpd.serve_forever()