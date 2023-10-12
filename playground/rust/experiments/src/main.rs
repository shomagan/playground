fn main() {
   let mut s0 = String::from("experiment zero");
   s0 = string_get_and_back_0(s0);
   println!("{}",s0);
   let mut s1 = String::from("experiment one");
   s1 = string_get_and_back_1(s1);
   println!("{}",s1);
   s1 = string_get_and_back_1(s0);//we lost s0 here 
   println!("{}",s1);
   let mut size_s1 = calculate_length(&s1);
   println!("size s1 is {}",size_s1);
   mut_string_references(&mut s1);//with this part we can not have more referenceses to s1
   let s2 = &s1;
   println!("s2 is {}",s2);
   println!("s1 after mut ref changes is {}",s1);
   size_s1 = calculate_length(&s1);
   println!("size s1 is {}",size_s1);
//-----------------------------------------------------------------------------
   const_size_values();
}
fn string_get_and_back_0(string_temp: String) -> String{
   string_temp
}
fn string_get_and_back_1(string_temp: String) -> String{
   return string_temp;
}
fn calculate_length(s: &String) -> usize {
   s.len()
}
fn mut_string_references(s: &mut String){
   s.push_str(", added");
}

fn const_size_values(){
   println!("const_size_values experiments");
   let x = 5;
   let y = x;
   println!("x is {}, y is {}",x ,y);
   get_reference_to_const_size_value(&x,y);
   println!("again x is {}, y is {}",x ,y);
}
fn get_reference_to_const_size_value(x_ref: &u8,y_copy: u8){
   println!("x_ref is {}, y_copy is {}",x_ref,y_copy);
}