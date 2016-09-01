class ChangeEvaluationField < ActiveRecord::Migration
  def change
    rename_column :evaluations, :funcionario_id, :employee_id
  end
end
