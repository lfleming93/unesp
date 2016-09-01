class CreateTreatments < ActiveRecord::Migration
  def change
    create_table :treatments do |t|
      t.references :installation, index: true, foreign_key: true
      t.references :employee, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
