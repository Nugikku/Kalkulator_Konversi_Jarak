#include "MyForm.h"
// MyForm.h
#pragma once

namespace KalkulatorKonversi {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			InisialisasiKonverter();
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::Windows::Forms::TextBox^ txtInput;
		System::Windows::Forms::ComboBox^ cmbDari;
		System::Windows::Forms::ComboBox^ cmbKe;
		System::Windows::Forms::Label^ lblHasil;
		System::Windows::Forms::Label^ lblDari;
		System::Windows::Forms::Label^ lblKe;
		Dictionary<String^, double>^ faktorKonversi;

		System::ComponentModel::Container^ components;

		void InitializeComponent(void)
		{
			this->txtInput = (gcnew System::Windows::Forms::TextBox());
			this->cmbDari = (gcnew System::Windows::Forms::ComboBox());
			this->cmbKe = (gcnew System::Windows::Forms::ComboBox());
			this->lblHasil = (gcnew System::Windows::Forms::Label());
			this->lblDari = (gcnew System::Windows::Forms::Label());
			this->lblKe = (gcnew System::Windows::Forms::Label());

			// Form
			this->SuspendLayout();
			this->Name = L"MyForm";
			this->Text = L"Kalkulator Konversi Satuan";
			this->ClientSize = System::Drawing::Size(484, 261);

			// Label Dari
			this->lblDari->AutoSize = true;
			this->lblDari->Location = System::Drawing::Point(12, 15);
			this->lblDari->Size = System::Drawing::Size(35, 16);
			this->lblDari->Text = L"Dari:";

			// Label Ke
			this->lblKe->AutoSize = true;
			this->lblKe->Location = System::Drawing::Point(12, 45);
			this->lblKe->Size = System::Drawing::Size(35, 16);
			this->lblKe->Text = L"Ke:";

			// Input TextBox
			this->txtInput->Location = System::Drawing::Point(168, 12);
			this->txtInput->Size = System::Drawing::Size(100, 22);
			this->txtInput->TextChanged += gcnew System::EventHandler(this, &MyForm::OnTextChanged);

			// ComboBox Dari
			this->cmbDari->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbDari->Location = System::Drawing::Point(53, 12);
			this->cmbDari->Size = System::Drawing::Size(109, 24);
			this->cmbDari->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::OnSelectionChanged);

			// ComboBox Ke
			this->cmbKe->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbKe->Location = System::Drawing::Point(53, 42);
			this->cmbKe->Size = System::Drawing::Size(109, 24);
			this->cmbKe->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::OnSelectionChanged);

			// Label Hasil
			this->lblHasil->AutoSize = true;
			this->lblHasil->Location = System::Drawing::Point(12, 75);
			this->lblHasil->Size = System::Drawing::Size(300, 16);
			this->lblHasil->Text = L"Hasil: ";

			// Add controls to form
			this->Controls->Add(this->txtInput);
			this->Controls->Add(this->cmbDari);
			this->Controls->Add(this->cmbKe);
			this->Controls->Add(this->lblHasil);
			this->Controls->Add(this->lblDari);
			this->Controls->Add(this->lblKe);

			this->ResumeLayout(false);
			this->PerformLayout();
		}

		void InisialisasiKonverter()
		{
			faktorKonversi = gcnew Dictionary<String^, double>();

			// Inisialisasi faktor konversi (relatif terhadap meter)
			faktorKonversi->Add("Meter", 1.0);
			faktorKonversi->Add("Kilometer", 0.001);
			faktorKonversi->Add("Centimeter", 100.0);
			faktorKonversi->Add("Millimeter", 1000.0);
			faktorKonversi->Add("Micrometer", 1000000.0);
			faktorKonversi->Add("Nanometer", 1000000000.0);
			faktorKonversi->Add("Mile", 0.000621371);
			faktorKonversi->Add("Yard", 1.09361);
			faktorKonversi->Add("Foot", 3.28084);
			faktorKonversi->Add("Inch", 39.3701);
			faktorKonversi->Add("Light Year", 1.057e-16);

			// Tambahkan item ke combo boxes
			for each (String ^ satuan in faktorKonversi->Keys)
			{
				cmbDari->Items->Add(satuan);
				cmbKe->Items->Add(satuan);
			}

			// Set nilai default
			if (cmbDari->Items->Count > 0)
				cmbDari->SelectedIndex = 0;
			if (cmbKe->Items->Count > 0)
				cmbKe->SelectedIndex = 2; // Default ke Centimeter
		}

		void HitungKonversi()
		{
			if (String::IsNullOrEmpty(txtInput->Text) ||
				cmbDari->SelectedItem == nullptr ||
				cmbKe->SelectedItem == nullptr)
				return;

			try {
				double nilai = System::Convert::ToDouble(txtInput->Text);
				String^ dari = safe_cast<String^>(cmbDari->SelectedItem);
				String^ ke = safe_cast<String^>(cmbKe->SelectedItem);

				double hasil = nilai * (faktorKonversi[ke] / faktorKonversi[dari]);

				lblHasil->Text = String::Format("Hasil: {0} {1} = {2} {3}",
					nilai.ToString("0.##"), dari, hasil.ToString("0.##"), ke);
			}
			catch (FormatException^) {
				lblHasil->Text = "Hasil: Masukkan angka yang valid";
			}
			catch (Exception^) {
				lblHasil->Text = "Hasil: Terjadi kesalahan dalam konversi";
			}
		}

		void OnTextChanged(System::Object^ sender, System::EventArgs^ e)
		{
			HitungKonversi();
		}

		void OnSelectionChanged(System::Object^ sender, System::EventArgs^ e)
		{
			HitungKonversi();
		}
	};
}
// Main.cpp
#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	KalkulatorKonversi::MyForm form;
	Application::Run(% form);
	return 0;
}