use reqwest::Client;
use serde::Serialize;

#[derive(Serialize)]
struct SendMessageRequest {
    chat_id: String,
    text: String,
}

#[tokio::main]
async fn main() -> Result<(), reqwest::Error> {
    // Replace <BOT_TOKEN> with your Telegram bot token
    let bot_token = "<BOT_TOKEN>";
    let chat_id = "<CHAT_ID>"; // Replace with the chat ID where you want to send the message

    let request = SendMessageRequest {
        chat_id: chat_id.to_string(),
        text: "Hello from Rust!".to_string(),
    };

    let client = Client::new();
    let response = client
        .post(&format!("https://api.telegram.org/bot{}/sendMessage", bot_token))
        .json(&request)
        .send()
        .await?;

    if response.status().is_success() {
        println!("Message sent successfully!");
    } else {
        println!("Failed to send message: {}", response.text().await?);
    }

    Ok(())
}