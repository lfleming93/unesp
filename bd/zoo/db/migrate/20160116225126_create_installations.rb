class CreateInstallations < ActiveRecord::Migration
  def change
    create_table :installations do |t|
      t.string :descricao
      t.decimal :area
      t.decimal :volume
      t.string :localizacao
      t.string :tipo
      t.references :sponsor, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
