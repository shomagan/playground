/*import longest_palindromic.rs */
mod longest_palindromic;

/*based on argument calls only needed functions */
fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        eprintln!("Please provide an argument");
        return;
    }

    match args[1].as_str() {
        "longest_palindromic" => {
            let result = longest_palindromic::longest_palindrome(args[2].clone());
            println!("Longest palindrome: {}", result);
        }
        _ => eprintln!("Unknown argument: {}", args[1]),
    }
}
