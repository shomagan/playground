# -*- coding: utf-8 -*-
from fpdf import FPDF
import requests
from PIL import Image
from io import BytesIO

fish_data = [
    ("Жерех", "до 50", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/6/6e/Aspius_aspius.jpg/320px-Aspius_aspius.jpg"),
    ("Озёрная кумжа", "до 50", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/9/9d/Salmo_trutta_lake.jpg/320px-Salmo_trutta_lake.jpg"),
    ("Брзана", "до 40", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/5/5e/Barbus_barbus.jpg/320px-Barbus_barbus.jpg"),
    ("Линь", "до 30", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/2/2c/Tinca_tinca.jpg/320px-Tinca_tinca.jpg"),
    ("Таймень", "до 70", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/3/3a/Hucho_taimen.jpg/320px-Hucho_taimen.jpg"),
    ("Язь", "до 25", "свыше 55", "https://upload.wikimedia.org/wikipedia/commons/thumb/7/7d/Leuciscus_idus.jpg/320px-Leuciscus_idus.jpg"),
    ("Клень", "до 25", "свыше 55", "https://upload.wikimedia.org/wikipedia/commons/thumb/4/4e/Squalius_cephalus.jpg/320px-Squalius_cephalus.jpg"),
    ("Елец", "до 15", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/1/1f/Leuciscus_leuciscus.jpg/320px-Leuciscus_leuciscus.jpg"),
    ("Краснопёрка", "до 15", "-", "https://upload.wikimedia.org/wikipedia/commons/thumb/1/1e/Scardinius_erythrophthalmus.jpg/320px-Scardinius_erythrophthalmus.jpg"),
    ("Карп", "до 30", "свыше 60", "https://upload.wikimedia.org/wikipedia/commons/thumb/3/3e/Cyprinus_carpio.jpg/320px-Cyprinus_carpio.jpg"),
]

pdf = FPDF()
pdf.set_auto_page_break(auto=True, margin=15)
pdf.add_page()
pdf.set_font("Arial", "B", 16)
pdf.cell(200, 10, "Охранные размеры рыб".encode("cp1251").decode('utf-8'), ln=True, align="C")

pdf.set_font("Arial", size=12)

for name, min_size, max_size, img_url in fish_data:
    pdf.ln(10)
    pdf.set_font("Arial", "B", 12)
    pdf.cell(0, 10, f"{name}", ln=True )
    pdf.set_font("Arial", size=12)
    pdf.cell(0, 10, f"Минимальный размер: {min_size} см | Максимальный размер: {max_size} см", ln=True)
    
    try:
        response = requests.get(img_url)
        img = Image.open(BytesIO(response.content))
        img_path = f"/tmp/{name}.jpg"
        img.save(img_path)
        pdf.image(img_path, w=60)
    except:
        pdf.cell(0, 10, "[Изображение недоступно]", ln=True)
pdf_path = "ryby_razmery.pdf"
pdf.output(pdf_path)
pdf_path