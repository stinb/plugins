import os
import openai

openai.api_key = os.getenv("OPENAI_API_KEY")
language = "C++"

contents = []
while True:
    try:
        line = input()
    except EOFError:
        break
    contents.append(line)


contents= '\n'.join(contents)
contents= "#" + language +"\n"+contents+"\n\n/* Explanation of what the code does\n\n"

response = openai.Completion.create(
  model="code-davinci-002",
  prompt=contents,
  temperature=0,
  max_tokens=128,
  top_p=1.0,
  frequency_penalty=0.0,
  presence_penalty=0.0,
  stop=["*/"]
)

print(response.choices[0].text)
