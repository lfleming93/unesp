require 'test_helper'

class EmployeesControllerTest < ActionController::TestCase
  setup do
    @employee = employees(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:employees)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create employee" do
    assert_difference('Employee.count') do
      post :create, employee: { bairro: @employee.bairro, cidade: @employee.cidade, data_cont: @employee.data_cont, data_nasc: @employee.data_nasc, especialidade: @employee.especialidade, funcao: @employee.funcao, n_carteira_trabalho: @employee.n_carteira_trabalho, nome: @employee.nome, rua: @employee.rua, sexo: @employee.sexo, telefone: @employee.telefone }
    end

    assert_redirected_to employee_path(assigns(:employee))
  end

  test "should show employee" do
    get :show, id: @employee
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @employee
    assert_response :success
  end

  test "should update employee" do
    patch :update, id: @employee, employee: { bairro: @employee.bairro, cidade: @employee.cidade, data_cont: @employee.data_cont, data_nasc: @employee.data_nasc, especialidade: @employee.especialidade, funcao: @employee.funcao, n_carteira_trabalho: @employee.n_carteira_trabalho, nome: @employee.nome, rua: @employee.rua, sexo: @employee.sexo, telefone: @employee.telefone }
    assert_redirected_to employee_path(assigns(:employee))
  end

  test "should destroy employee" do
    assert_difference('Employee.count', -1) do
      delete :destroy, id: @employee
    end

    assert_redirected_to employees_path
  end
end
